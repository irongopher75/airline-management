const express = require('express');
const bodyParser = require('body-parser');
const cors = require('cors');
const flightsRouter = require('./routes/flights');
const path = require('path');

// Initialize the app
const app = express();
const PORT = 5001;

// Middleware
app.use(cors());
app.use(bodyParser.json());

// Serve static frontend files
app.use('/', express.static(path.join(__dirname, '../airline-management-frontend')));

// Routes
app.use('/api/flights', flightsRouter);

// Server
app.listen(PORT, () => {
  console.log(`Server running on http://localhost:${PORT}`);
}).on('error', (err) => {
  if (err.code === 'EADDRINUSE') {
    console.log(`Port ${PORT} is already in use, trying another port...`);
    process.exit(1);  // Exit the process so you can try with a different port
  }
});
