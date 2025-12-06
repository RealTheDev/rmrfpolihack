require('dotenv').config();
const express = require('express');
const mongoose = require('mongoose');
const cors = require('cors');

const app = express();
app.use(cors());
app.use(express.json());

// MongoDB Connection
mongoose.connect(process.env.MONGO_URI)
    .then(() => console.log('MongoDB Connected'))
    .catch(err => console.log(err));

// Data Model
const binSchema = new mongoose.Schema({
    binId: { type: String, unique: true, required: true },
    latitude: { type: Number, required: true },
    longitude: { type: Number, required: true },
    fillLevel: { type: Number, default: 0 },
    lastUpdated: { type: Date, default: Date.now }
});

const Bin = mongoose.model('Bin', binSchema);

// API Endpoints

// 1. Create New Bin
app.post('/api/bins', async (req, res) => {
    const { binId, lat, lng } = req.body;

    try {
        const existingBin = await Bin.findOne({ binId });
        if (existingBin) {
            return res.status(409).json({ message: 'Bin already exists' });
        }

        const newBin = new Bin({
            binId,
            latitude: lat,
            longitude: lng
        });

        await newBin.save();
        res.status(201).json(newBin);
    } catch (error) {
        res.status(500).json({ message: error.message });
    }
});

// 2. Sensor Update
app.put('/api/bins', async (req, res) => {
    const { binId, fillLevel } = req.body;

    try {
        const bin = await Bin.findOne({ binId });

        if (!bin) {
            return res.status(404).json({ message: 'Bin not found' });
        }

        bin.fillLevel = fillLevel;
        bin.lastUpdated = Date.now();
        await bin.save();

        res.json(bin);
    } catch (error) {
        res.status(500).json({ message: error.message });
    }
});

// 3. List All Bins
app.get('/api/bins', async (req, res) => {
    console.log("get all bins")
    try {
        const bins = await Bin.find();
        res.json(bins);
    } catch (error) {
        res.status(500).json({ message: error.message });
    }
});

const PORT = process.env.PORT || 5000;
app.listen(PORT, () => console.log(`Server running on port ${PORT}`));
