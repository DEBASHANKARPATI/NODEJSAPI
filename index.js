const express = require('express');
const http = require('http');
const { join } = require('path');
const WebSocket = require('ws');
const cors = require('cors')

const app = express();
const server = http.createServer(app);
const wss = new WebSocket.Server({ server });
app.use(express.json())
app.use(cors())
// WebSocket endpoint
wss.on('connection', (ws) => {
  console.log('Client connected');

  ws.on('message', (message) => {
    const requestData = JSON.parse(message);

    if (requestData.method === 'GET') {
      // Handle GET request
      const responseData = { message: 'GET response' };
      ws.send(JSON.stringify(responseData));
    } else if (requestData.method === 'POST') {
      // Handle POST request
      // Perform necessary processing with requestData.body
      const responseData = { message: 'POST response' };
      ws.send(JSON.stringify(responseData));
    }
  });

  ws.on('close', () => {
    console.log('Client disconnected');
  });
});

// HTTP endpoint
app.get('/api', (req, res) => {
  const responseData = { message: 'HTTP endpoint response' };
  res.json(responseData);
});

server.listen(8080, () => {
  console.log('Server listening on port 8080');
});

