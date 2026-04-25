import { Server } from 'http';

export async function teardownConnection(server: Server): Promise<void> {
    return new Promise((resolve, reject) => {
        if (!server) {
            resolve();
            return;
        }

        // Close server to stop accepting new connections
        server.close((err) => {
            if (err) {
                // If it fails, we reject
                reject(err);
            } else {
                resolve();
            }
        });

        // Forcefully close connections to ensure we release the port.
        // Node 18.2.0+ supports server.closeAllConnections()
        if ('closeAllConnections' in server) {
            (server as any).closeAllConnections();
        }
    });
}
