import * as grpc from '@grpc/grpc-js';
import * as protoLoader from '@grpc/proto-loader';
import path from 'path';
import { scenarios } from './scenarios';
import { BaseScenario } from './scenarios/BaseScenario';
import { connectToDatabase, getClient } from './db/connection';

const PROTO_PATH = path.join(__dirname, 'protos', 'orchestrator.proto');
const packageDefinition = protoLoader.loadSync(PROTO_PATH, {
    keepCase: true,
    longs: String,
    enums: String,
    defaults: true,
    oneofs: true
});
const protoDescriptor = grpc.loadPackageDefinition(packageDefinition) as any;
const orchestratorProto = protoDescriptor.orchestrator;

let currentScenarioName: string | null = null;
let currentScenario: BaseScenario | null = null;

async function clearDatabase() {
    const db = await connectToDatabase();
    await db.dropDatabase();
}

const server = new grpc.Server();

server.addService(orchestratorProto.OrchestratorService.service, {
    List: (call: any, callback: any) => {
        const scenarioList = Object.entries(scenarios).map(([id, scenario]) => ({
            name: id,
            description: scenario.description,
            sampleRequest: scenario.sampleRequest
        }));
        callback(null, { scenarios: scenarioList });
    },
    
    Status: (call: any, callback: any) => {
        callback(null, {
            current_scenario: currentScenarioName || "",
            running: currentScenario !== null
        });
    },

    Run: async (call: any, callback: any) => {
        const { scenario_name, force } = call.request;
        
        if (currentScenario && !force) {
            callback(null, { success: false, message: `Scenario ${currentScenarioName} is currently running. Stop it first.` });
            return;
        }

        const nextScenario = scenarios[scenario_name];
        if (!nextScenario) {
            callback(null, { success: false, message: `Scenario ${scenario_name} not found.` });
            return;
        }

        try {
            if (currentScenario) {
                await currentScenario.teardown();
                currentScenario = null;
                currentScenarioName = null;
            }

            await clearDatabase();
            await nextScenario.seed();
            await nextScenario.setup();

            currentScenario = nextScenario;
            currentScenarioName = scenario_name;

            callback(null, { success: true, message: `Scenario ${scenario_name} started successfully.` });
        } catch (error: any) {
            console.error('Error starting scenario:', error);
            callback(null, { success: false, message: `Failed to start scenario: ${error.message}` });
        }
    },

    Stop: async (call: any, callback: any) => {
        if (!currentScenario) {
            callback(null, { success: true, message: 'No scenario is currently running.' });
            return;
        }

        try {
            const name = currentScenarioName;
            await currentScenario.teardown();
            await clearDatabase();
            
            currentScenario = null;
            currentScenarioName = null;

            callback(null, { success: true, message: `Scenario ${name} stopped successfully.` });
        } catch (error: any) {
            console.error('Error stopping scenario:', error);
            callback(null, { success: false, message: `Failed to stop scenario: ${error.message}` });
        }
    }
});

const PORT = 9000;
server.bindAsync(`0.0.0.0:${PORT}`, grpc.ServerCredentials.createInsecure(), (err, port) => {
    if (err) {
        console.error('Failed to bind gRPC server', err);
        return;
    }
    console.log(`Control API listening on port ${port}`);
});
