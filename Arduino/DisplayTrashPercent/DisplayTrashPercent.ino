#include "Arduino_LED_Matrix.h"
#include "trash_icons.h" 
#include "WiFiS3.h"

#define TRIG_PIN 10
#define ECHO_PIN 9

// Settings for the filtering algorithm
const int SAMPLE_SIZE = 20;
const int IGNORE_LOW = 5;
const int IGNORE_HIGH = 5;


ArduinoLEDMatrix matrix;
// ---------------------------------------------------------------------------
// START ULTRASONIC FUNCTIONS
// ---------------------------------------------------------------------------


// Array to store data samples. 
// Note: We pass this array into functions rather than relying on a global variable where possible, 
// but keeping it global here prevents stack overflow if the array gets very large.
float filterArray[SAMPLE_SIZE];

// 1. Setup Function: Handles pin configurations
void setupSensor() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

// 2. Main Measurement Logic: Handles sampling, sorting, and averaging
float getFilteredDistance() {
  
  // Step A: Take samples
  for (int sample = 0; sample < SAMPLE_SIZE; sample++) {
    filterArray[sample] = performSingleMeasurement();
    delay(30); // delay to avoid echo interference
  }

  // Step B: Sort array (Bubble Sort)
  sortArray(filterArray, SAMPLE_SIZE);

  // Step C: Calculate average of the middle samples
  // We start at IGNORE_LOW (index 5) and stop before SAMPLE_SIZE - IGNORE_HIGH (index 15)
  double sum = 0;
  int count = 0;
  
  for (int sample = IGNORE_LOW; sample < (SAMPLE_SIZE - IGNORE_HIGH); sample++) {
    sum += filterArray[sample];
    count++;
  }

  return sum / count;
}

// 3. Helper: Performs a single ping (Hardware interaction)
float performSingleMeasurement() {
  digitalWrite(TRIG_PIN, LOW); // Ensure clean low before high
  delayMicroseconds(2);
  
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  float duration_us = pulseIn(ECHO_PIN, HIGH);
  
  // Calculate distance: (duration / 2) * speed_of_sound (0.0343 cm/us)
  // or simply duration * 0.017
  return 0.017 * duration_us;
}

// 4. Helper: Sorts an array (Logic utility)
void sortArray(float *arr, int size) {
  for (int i = 0; i < size - 1; i++) {
    for (int j = i + 1; j < size; j++) {
      if (arr[i] > arr[j]) {
        float swap = arr[i];
        arr[i] = arr[j];
        arr[j] = swap;
      }
    }
  }
}

// 5. Output: Handles Serial printing
void printDistance(float dist) {
  Serial.print("Distance: ");
  Serial.print(dist);
  Serial.println(" cm");
}

// ---------------------------------------------------------------------------
// END ULTRASONIC FUNCTIONS
// ---------------------------------------------------------------------------



// ---------------------------------------------------------------------------
// ENTER YOUR WIFI CREDENTIALS HERE
// ---------------------------------------------------------------------------
char ssid[] = "cloudflight-guest";        // your network SSID (name)
char pass[] = "digitalfuture";    // your network password (use for WPA, or use as key for WEP)


// ---------------------------------------------------------------------------
// SETTINGS
// ---------------------------------------------------------------------------
int status = WL_IDLE_STATUS;
char serverAddress[] = "rmrfpolihack.onrender.com";    // The database link


// Use SSL Client for HTTPS
WiFiSSLClient client;


// ---------------------------------------------------------------------------
// HELPER: Print WiFi details
// ---------------------------------------------------------------------------
void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}


// ---------------------------------------------------------------------------
// FUNCTION: PUT Request with JSON Data
// ---------------------------------------------------------------------------
void sendFillLevel(int level) {
  
  // 1. Create the JSON string dynamically
  // We use String object here for easier concatenation of the variable
  String jsonBody = "{\"binId\": \"BIN_02\", \"fillLevel\": " + String(level) + "}";
  
  Serial.println("\nConnecting to server...");

  // 2. Connect to Port 443 (HTTPS)
  if (client.connect(serverAddress, 443)) {
    Serial.println("Connected. Sending PUT request...");

    // 3. Send HTTP Headers
    client.println("PUT /api/bins HTTP/1.1");
    client.print("Host: "); 
    client.println(serverAddress);
    client.println("Content-Type: application/json");
    
    // Calculate and send the length of the JSON payload
    client.print("Content-Length: ");
    client.println(jsonBody.length());
    
    // Close connection after response
    client.println("Connection: close"); 
    
    // Empty line to separate headers from body
    client.println(); 
    
    // 4. Send the JSON Body
    client.println(jsonBody);

  } else {
    Serial.println("Connection failed!");
    return;
  }

  // 5. Read the Server's Response (Optional but good for debugging)
  // This waits for the server to reply and prints it to Serial
  while (client.connected() || client.available()) {
    if (client.available()) {
      char c = client.read();
      Serial.write(c);
    }
  }

  // 6. Close the client
  client.stop();
  Serial.println("\nRequest complete.");
}



// ---------------------------------------------------------------------------
// HELPER: 
// ---------------------------------------------------------------------------

void display_trash_percent(float distance_cm) {
  
  // 1. Logic for 100% (Full)
  // If trash is within 4cm of the sensor (or closer)
  if (distance_cm <= 4.0) {
    sendFillLevel(100);
    matrix.loadFrame(Trash_can_100);
    Serial.print("Displaying: 100% | Dist: ");
    Serial.println(distance_cm);
    
  } else if (distance_cm <= 8.75) {
    // If trash is between 4cm and 8.75cm
    sendFillLevel(87.5);
    matrix.loadFrame(Trash_can_87_5);
    Serial.print("Displaying: 87.5% | Dist: ");
    Serial.println(distance_cm);
    
  } else if (distance_cm <= 17.5) {
    sendFillLevel(75);
    matrix.loadFrame(Trash_can_75);
    Serial.print("Displaying: 75% | Dist: ");
    Serial.println(distance_cm);
    
  } else if (distance_cm <= 26.25) { 
    sendFillLevel(62.5);
    matrix.loadFrame(Trash_can_62_5); 
    Serial.print("Displaying: 62.5% | Dist: ");
    Serial.println(distance_cm);
    
  } else if (distance_cm <= 35.0) {
    sendFillLevel(50);
    matrix.loadFrame(Trash_can_50);
    Serial.print("Displaying: 50% | Dist: ");
    Serial.println(distance_cm);
    
  } else if (distance_cm <= 43.75) {
    sendFillLevel(37.5);
    matrix.loadFrame(Trash_can_37_5); 
    Serial.print("Displaying: 37.5% | Dist: ");
    Serial.println(distance_cm);
    
  } else if (distance_cm <= 52.5) {
    sendFillLevel(25);
    matrix.loadFrame(Trash_can_25);
    Serial.print("Displaying: 25% | Dist: ");
    Serial.println(distance_cm);
    
  } else if (distance_cm <= 61.25) {
    sendFillLevel(12.5);
    matrix.loadFrame(Trash_can_12_5);
    Serial.print("Displaying: 12.5% | Dist: ");
    Serial.println(distance_cm);
  
  } else if (distance_cm <= 75) {
    sendFillLevel(0);
    matrix.loadFrame(Trash_can_0);
    Serial.print("Displaying: 0% | Dist: ");
    Serial.println(distance_cm);
  
  } else {
    // The bin has 75 cm depth. The sensor might show values above 100 when is to close to the sensor. (below 2 cm)
    sendFillLevel(100);
    matrix.loadFrame(Trash_can_100);
    Serial.print("(FALSE DISTANCE) Displaying: 100% | Dist: ");
    Serial.println(distance_cm);
  }
}


void setup() {
  Serial.begin(115200);
  setupSensor();
  matrix.begin();
  while (!Serial); // Wait for Serial Monitor

  // 1. Check Module
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }

  // 2. Connect to WiFi
  Serial.print("Connecting to SSID: ");
  Serial.println(ssid);
  
  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED) {
    status = WiFi.begin(ssid, pass);
    delay(10000); // Wait 10 seconds for connection
  }
  Serial.println("Connected to WiFi.");
  printWifiStatus();
  
}


void loop() {
    
    Serial.println("\n--- Enter Fill Level (0-100) in the input box above ---");

    // Seonsor
    float currentDistance = getFilteredDistance();
    printDistance(currentDistance);  

    // Update the Display
    display_trash_percent(currentDistance);
    
    // Wait 10 seconds before running again so we don't flood the server
    delay(10000);
}