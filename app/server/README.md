# IoT Smart Bins Backend

This is the backend service for the IoT Smart Bins hackathon project. It creates a REST API using Node.js, Express, and MongoDB.

## Installation

1.  Clone the repository.
2.  Install dependencies:
    ```bash
    npm install
    ```
3.  Configure environment variables in `.env`:
    ```
    PORT=5000
    MONGO_URI=...
    ```

## Running the Server

Start the server using:
```bash
npm start
```
Or for development:
```bash
node server.js
```

## API Endpoints

### 1. Create New Bin
Used when installing a new bin in the network.

- **URL:** `/api/bins`
- **Method:** `POST`
- **Body:**
  ```json
  {
    "binId": "BIN_05",
    "lat": 46.7712,
    "lng": 23.6236
  }
  ```
- **Success Response:** `201 Created`
- **Error Response:** `409 Conflict` (if Bin ID already exists)

### 2. Update Sensor Data
Called by the ESP32 device to update fill levels.

- **URL:** `/api/bins`
- **Method:** `PUT`
- **Body:**
  ```json
  {
    "binId": "BIN_05",
    "fillLevel": 80
  }
  ```
- **Success Response:** `200 OK`
- **Error Response:** `404 Not Found`

### 3. List All Bins
Used by the frontend map to display all bins.

- **URL:** `/api/bins`
- **Method:** `GET`
- **Success Response:** `200 OK`
- **Response Example:**
  ```json
  [
    {
      "_id": "60d...",
      "binId": "BIN_05",
      "latitude": 46.7712,
      "longitude": 23.6236,
      "fillLevel": 80,
      "lastUpdated": "2023-10-27T10:00:00.000Z"
    },
    {
      "_id": "60d...",
      "binId": "BIN_06",
      "latitude": 46.7712,
      "longitude": 23.6236,
      "fillLevel": 80,
      "lastUpdated": "2023-10-27T10:00:00.000Z"
    }
  ]
  ```
