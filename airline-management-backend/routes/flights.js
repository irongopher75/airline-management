const express = require('express');
const router = express.Router();
const fs = require('fs');
const path = require('path');

const dataPath = path.join(__dirname, '../data/flights.json');

// Utility to read/write data
const readData = () => JSON.parse(fs.readFileSync(dataPath));
const writeData = (data) => fs.writeFileSync(dataPath, JSON.stringify(data, null, 2));

// Get all flights
router.get('/', (req, res) => {
  const flights = readData();
  res.json(flights);
});

// Add a new flight
router.post('/add', (req, res) => {
  const { company, flightNumber, source, destination, totalSeats } = req.body;
  if (!company || !flightNumber || !source || !destination || !totalSeats) {
    return res.status(400).json({ message: 'Missing required flight details' });
  }
  const flights = readData();
  const newFlight = {
    id: Date.now().toString(),
    company,
    flightNumber,
    source,
    destination,
    totalSeats: parseInt(totalSeats),
    availableSeats: parseInt(totalSeats),
    passengers: [],
  };
  flights.push(newFlight);
  writeData(flights);
  res.status(201).json({ message: 'Flight added successfully', flight: newFlight });
});

// Reserve a seat
router.post('/:flightId/reserve', (req, res) => {
  const { flightId } = req.params;
  const { passenger } = req.body;

  if (!passenger) {
    return res.status(400).json({ message: 'Passenger name is required' });
  }

  const flights = readData();
  const flight = flights.find((f) => f.id === flightId);

  if (!flight) {
    return res.status(404).json({ message: 'Flight not found' });
  }

  if (flight.availableSeats > 0) {
    flight.passengers.push(passenger);
    flight.availableSeats -= 1;
    writeData(flights);
    res.status(200).json({ message: 'Seat reserved successfully', flight });
  } else {
    res.status(400).json({ message: 'No seats available' });
  }
});

// Cancel a reservation
router.post('/:flightId/cancel', (req, res) => {
  const { flightId } = req.params;
  const { passenger } = req.body;

  const flights = readData();
  const flight = flights.find((f) => f.id === flightId);

  if (!flight) {
    return res.status(404).json({ message: 'Flight not found' });
  }

  const passengerIndex = flight.passengers.indexOf(passenger);

  if (passengerIndex === -1) {
    return res.status(400).json({ message: 'Passenger not found' });
  }

  flight.passengers.splice(passengerIndex, 1);
  flight.availableSeats += 1;
  writeData(flights);

  res.status(200).json({ message: 'Reservation canceled successfully', flight });
});

// Get passengers for a specific flight
router.get('/:flightId/passengers', (req, res) => {
  const { flightId } = req.params;
  const flights = readData();
  const flight = flights.find((f) => f.id === flightId);

  if (!flight) {
    return res.status(404).json({ message: 'Flight not found' });
  }

  res.json(flight.passengers);
});

module.exports = router;
