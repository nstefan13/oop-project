import { Server } from 'http';
import { teardownConnection } from '../utils/server-helper';

export abstract class BaseScenario {
    abstract name: string;
    abstract description: string;
    abstract sampleRequest: string;
    
    protected server: Server | null = null;

    // Seeds the database for this scenario
    abstract seed(): Promise<void>;

    // Sets up the express server on port 8000
    abstract setup(): Promise<void>;

    // Tears down the server
    async teardown(): Promise<void> {
        if (this.server) {
            await teardownConnection(this.server);
            this.server = null;
        }
    }
}
