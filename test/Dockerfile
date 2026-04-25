FROM node:20-alpine

WORKDIR /app

COPY package.json package-lock.json* ./
RUN npm install

COPY . .

# Compile TypeScript
RUN npx tsc
RUN mkdir -p dist/protos && cp protos/orchestrator.proto dist/protos/

# Start the gRPC Control API server
CMD ["node", "dist/control-api.js"]
