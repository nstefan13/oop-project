import express, { Request, Response } from 'express';
import { BaseScenario } from './BaseScenario';
import { connectToDatabase } from '../db/connection';

export class WhereClauseScenario extends BaseScenario {
    name = 'WhereClauseInjection';
    description = 'A POST route that searches for a user using a raw $where clause. Vulnerable to NoSQL injection.';
    
    // Sample request showing the injection point in the body
    sampleRequest = `POST / HTTP/1.1
Host: localhost:8000
Content-Type: application/json
Accept: */*

{
  "username": "{{payload}}",
  "password": "unknown"
}`;

    async seed(): Promise<void> {
        const db = await connectToDatabase();
        const users = db.collection('users');
        await users.insertMany([
            { username: 'admin', password: 'supersecretpassword123', role: 'admin' },
            { username: 'user1', password: 'password1', role: 'user' },
        ]);
    }

    async setup(): Promise<void> {
        const app = express();
        app.use(express.json());

        app.post('/', async (req: Request, res: Response) => {
            const { username, password } = req.body;

            try {
                const db = await connectToDatabase();
                const users = db.collection('users');

                // VULNERABLE QUERY: using $where with user input directly concatenated
                const query = {
                    $where: `this.username == '${username}' && this.password == '${password}'`
                };

                const user = await users.findOne(query);

                if (user) {
                    res.status(200).json({ message: 'Login successful', user: { username: user.username, role: user.role } });
                } else {
                    res.status(401).json({ message: 'Invalid credentials' });
                }
            } catch (err: any) {
                res.status(500).json({ error: 'Internal Server Error' });
            }
        });

        return new Promise((resolve) => {
            this.server = app.listen(8000, () => {
                resolve();
            });
        });
    }
}
