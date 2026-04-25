import { MongoClient, Db } from 'mongodb';

const url = process.env.MONGO_URL || 'mongodb://localhost:27017';
const client = new MongoClient(url);

let dbInstance: Db | null = null;

export async function connectToDatabase(): Promise<Db> {
    if (dbInstance) {
        return dbInstance;
    }
    await client.connect();
    dbInstance = client.db('vulndb');
    return dbInstance;
}

export async function getClient(): Promise<MongoClient> {
    return client;
}
