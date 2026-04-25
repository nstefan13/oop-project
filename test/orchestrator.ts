import * as grpc from '@grpc/grpc-js';
import * as protoLoader from '@grpc/proto-loader';
import path from 'path';
import { Command } from 'commander';
import prompts from 'prompts';

const PROTO_PATH = path.join(__dirname, 'protos', 'orchestrator.proto');
const packageDefinition = protoLoader.loadSync(PROTO_PATH, {
    keepCase: true,
    longs: String,
    enums: String,
    defaults: true,
    oneofs: true
});
const protoDescriptor = grpc.loadPackageDefinition(packageDefinition) as any;
const client = new protoDescriptor.orchestrator.OrchestratorService(
    'localhost:9000',
    grpc.credentials.createInsecure()
);

const program = new Command();

program
    .name('orchestrator')
    .description('CLI to manage vulnerable scenarios');

program
    .command('list')
    .description('List all available scenarios')
    .action(() => {
        client.List({}, (err: any, response: any) => {
            if (err) {
                console.error('Error fetching scenarios:', err.message);
                return;
            }
            console.log('Available Scenarios:\n');
            response.scenarios.forEach((s: any) => {
                console.log(`- ${s.name}: ${s.description}`);
            });
        });
    });

program
    .command('show <scenario>')
    .description('Show the raw sample HTTP request for a scenario')
    .action((scenario) => {
        client.List({}, (err: any, response: any) => {
            if (err) {
                console.error('Error fetching scenarios:', err.message);
                return;
            }
            const s = response.scenarios.find((s: any) => s.name === scenario);
            if (!s) {
                console.error(`Scenario '${scenario}' not found.`);
                return;
            }
            console.log(s.sampleRequest);
        });
    });

program
    .command('run <scenario>')
    .description('Run a specific scenario')
    .action(async (scenario) => {
        const runScenario = (force: boolean) => {
            client.Run({ scenario_name: scenario, force }, (err: any, response: any) => {
                if (err) {
                    console.error('Error running scenario:', err.message);
                    return;
                }
                if (response.success) {
                    console.log(response.message);
                } else {
                    console.error('Error:', response.message);
                }
            });
        };

        client.Status({}, async (err: any, response: any) => {
            if (err) {
                console.error('Error checking status:', err.message);
                return;
            }
            if (response.running && response.current_scenario !== scenario) {
                const { confirm } = await prompts({
                    type: 'confirm',
                    name: 'confirm',
                    message: `Scenario '${response.current_scenario}' is currently running. Do you want to stop it and start '${scenario}'?`,
                    initial: true
                });
                
                if (confirm) {
                    console.log(`Stopping '${response.current_scenario}'...`);
                    client.Stop({}, (err2: any, res2: any) => {
                        if (err2 || !res2.success) {
                            console.error('Failed to stop current scenario:', err2?.message || res2?.message);
                            return;
                        }
                        console.log(res2.message);
                        runScenario(true);
                    });
                } else {
                    console.log('Aborted.');
                }
            } else {
                runScenario(true); // force true just to bypass server-side check, since we did the client check
            }
        });
    });

program
    .command('stop')
    .description('Stop the currently running scenario')
    .action(() => {
        client.Stop({}, (err: any, response: any) => {
            if (err) {
                console.error('Error stopping scenario:', err.message);
                return;
            }
            console.log(response.message);
        });
    });

program.parse();
