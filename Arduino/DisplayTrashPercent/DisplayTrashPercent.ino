#include "Arduino_LED_Matrix.h"
#include "trash_icons.h" 
#include "WiFiS3.h"
#include <Adafruit_NeoPixel.h>
#include <Servo.h>

#define TRIG_PIN 3
#define ECHO_PIN 4
#define TRIG_PIN2 5
#define ECHO_PIN2 7

#define SERVO_1 10
#define SERVO_2 11

#define PIN_NEO_PIXEL  6   // Arduino pin that connects to NeoPixel
#define NUM_PIXELS     26  // The number of LEDs (pixels) on NeoPixel

Adafruit_NeoPixel NeoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);

Servo myservo;
Servo myservo2;unsigned long lastSensorTime = 0;   // Tracks the last time we read sensors
const long sensorInterval = 10000;  // 10 seconds in milliseconds


// Settings for the filtering algorithm
const int SAMPLE_SIZE = 20;
const int IGNORE_LOW = 5;
const int IGNORE_HIGH = 5;

ArduinoLEDMatrix matrix;

// Sensors timer for void loop
unsigned long lastSensorTimeSensors = 0;   // Tracks the last time we read sensors
const long sensorIntervalSensors = 10000;  // 10 seconds in milliseconds

// Array to store data samples. 
float filterArray[SAMPLE_SIZE];

float correctDistance;

// --- WIFI SETTINGS ---
char ssid[] = "HUAWEI P30 Pro";       // Your Network Name
char pass[] = "cdd82acc";           // Your Network Password

int status = WL_IDLE_STATUS;
char serverAddress[] = "rmrfpolihack.onrender.com";    
WiFiServer server(80); // Open Port 80 for Python commands
WiFiSSLClient client;  // For cloud upload

// --- TIMERS ---
unsigned long lastCloudUpload = 0;
const long CLOUD_INTERVAL = 5000; // Upload to cloud every 5 seconds (prevents lag)

// ---------------------------------------------------------------------------
// START ULTRASONIC FUNCTIONS
// ---------------------------------------------------------------------------
void setupSensor() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIG_PIN2, OUTPUT);
  pinMode(ECHO_PIN2, INPUT);
}

float performSingleMeasurement(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  float duration_us = pulseIn(echo, HIGH);
  return 0.017 * duration_us;
}

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

float getFilteredDistance(int trig, int echo) {
  for (int sample = 0; sample < SAMPLE_SIZE; sample++) {
    filterArray[sample] = performSingleMeasurement(trig, echo);
    delay(5); // Reduced delay to make loop faster
  }
  sortArray(filterArray, SAMPLE_SIZE);
  double sum = 0;
  int count = 0;
  for (int sample = IGNORE_LOW; sample < (SAMPLE_SIZE - IGNORE_HIGH); sample++) {
    sum += filterArray[sample];
    count++;
  }
  return sum / count;
}

void printDistance(float dist) {
  Serial.print("Distance: ");
  Serial.print(dist);
  Serial.println(" cm");
}

// ---------------------------------------------------------------------------
// CLOUD FUNCTIONS
// ---------------------------------------------------------------------------
void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void sendFillLevel(int level) {
  String jsonBody = "{\"binId\": \"BIN_01\", \"fillLevel\": " + String(level) + "}";
  Serial.println("\n[Cloud] Connecting to server...");
  Serial.print("Level: "); 
  Serial.println(level);

  if (client.connect(serverAddress, 443)) {
    Serial.println("[Cloud] Connected. Sending PUT...");
    client.println("PUT /api/bins HTTP/1.1");
    client.print("Host: "); 
    client.println(serverAddress);
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(jsonBody.length());
    client.println("Connection: close"); 
    client.println(); 
    client.println(jsonBody);

    // 2. WAIT for the server to process it (The missing piece)
    int timeout = 0;
    while (!client.available() && timeout < 5000) {
      delay(10);
      timeout += 10;
    }
  } else {
    Serial.println("[Cloud] Connection failed!");
    return;
  }
  client.stop();
  Serial.println("[Cloud] Request complete.");
}

// ---------------------------------------------------------------------------
// DISPLAY LOGIC
// ---------------------------------------------------------------------------
void display_trash_percent(float distance_cm) {
  int fillPercent = 0;
  if (distance_cm <= 3.75) {fillPercent = 100; matrix.loadFrame(Trash_can_100);}
  else if (distance_cm <= 8.75) { fillPercent = 87; matrix.loadFrame(Trash_can_87_5); }
  else if (distance_cm <= 17.5) { fillPercent = 75; matrix.loadFrame(Trash_can_75); }
  else if (distance_cm <= 26.25) { fillPercent = 62; matrix.loadFrame(Trash_can_62_5); }
  else if (distance_cm <= 35.0) { fillPercent = 50; matrix.loadFrame(Trash_can_50); }
  else if (distance_cm <= 43.75) { fillPercent = 37; matrix.loadFrame(Trash_can_37_5); }
  else if (distance_cm <= 52.5) { fillPercent = 25; matrix.loadFrame(Trash_can_25); }
  else if (distance_cm <= 61.25) { fillPercent = 12; matrix.loadFrame(Trash_can_12_5); }
  else if (distance_cm <= 80) { fillPercent = 0; matrix.loadFrame(Trash_can_0); }
  else{fillPercent = 100; matrix.loadFrame(Trash_can_100);}

  // ONLY send to cloud if timer expires (Non-blocking logic)
  if (millis() - lastCloudUpload > CLOUD_INTERVAL) {
     sendFillLevel(fillPercent);
     lastCloudUpload = millis();
  }
}

// ---------------------------------------------------------------------------
// MOTOR LOGIC
// ---------------------------------------------------------------------------
void RotateMotors(char direction) { 
  if (direction == 'r') {
    Serial.println("Executing RECYCLABLE Routine (Right)...");
    myservo.write(45);   myservo2.write(135);
    for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {           // for each pixel
    NeoPixel.setPixelColor(pixel, NeoPixel.Color(50, 0, 0));  // it only takes effect if pixels.show() is called
    NeoPixel.show();                                           // send the updated pixel colors to the NeoPixel hardware.
    delay(500/NUM_PIXELS);  // pause between each pixel
  }
    myservo.write(90);   myservo2.write(90);  delay(1000); 
    NeoPixel.clear();
    NeoPixel.show();
    myservo.write(135);  myservo2.write(45);  delay(320); 
    myservo.write(90);   myservo2.write(90);  delay(100); 
    Serial.println("Done.");
  } else if (direction == 'l') {
    Serial.println("Executing TRASH Routine (Left)...");
    myservo.write(135);  myservo2.write(45);
    for (int pixel = NUM_PIXELS-1; pixel >=0 ; pixel--) {           // for each pixel
    NeoPixel.setPixelColor(pixel, NeoPixel.Color(0, 50, 0));  // it only takes effect if pixels.show() is called
    NeoPixel.show();                                           // send the updated pixel colors to the NeoPixel hardware.
    delay(500/NUM_PIXELS);  // pause between each pixel
  }
    myservo.write(90);   myservo2.write(90);  delay(1000);
    NeoPixel.clear();
    NeoPixel.show();
    myservo.write(45);   myservo2.write(135); delay(320); 
    myservo.write(90);   myservo2.write(90);  delay(100);
    Serial.println("Done.");
  }
}

// Output the correct sensor data
float compareSensors(float sensor1, float sensor2) {
  float correctPercent;

  if (sensor1 >= 80) {
    Serial.print("Sensor1 is ");
    Serial.println(sensor1);
    correctPercent = sensor1;
  } 
  else if (sensor2 >= 80) {
    Serial.print("Sensor2 is ");
    Serial.println(sensor2);
    correctPercent = sensor2;
  } 
  else if (sensor1 < sensor2) {
    Serial.print("Sensor1 is ");
    Serial.println(sensor1);
    correctPercent = sensor1;
  } 
  else {
    Serial.print("Sensor2 is ");
    Serial.println(sensor2);
    correctPercent = sensor2;
  }

  return correctPercent;
}

// ---------------------------------------------------------------------------
// MAIN SETUP
// ---------------------------------------------------------------------------
void setup() {
  Serial.begin(115200); // Standard baud rate
  
  myservo.attach(SERVO_1);  
  myservo.write(90);  // stop
  myservo2.attach(SERVO_2); 
  myservo2.write(90); // stop

  NeoPixel.begin();  // INITIALIZE NeoPixel strip object
  setupSensor();
  matrix.begin();

  // WiFi Connection
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }

  Serial.print("Connecting to SSID: ");
  Serial.println(ssid);
  
  while (status != WL_CONNECTED) {
    status = WiFi.begin(ssid, pass);
    delay(5000);
  }
  
  Serial.println("Connected to WiFi.");
  printWifiStatus();
  
  server.begin(); // Start listening for Python commands
  Serial.println("Server started. Listening for commands...");

  // First percent of the bin
  Serial.println("Taking initial reading...");
  float d1 = getFilteredDistance(TRIG_PIN, ECHO_PIN);
  float d2 = getFilteredDistance(TRIG_PIN2, ECHO_PIN2);
  correctDistance = compareSensors(d1, d2);

}

// ---------------------------------------------------------------------------
// MAIN LOOP
// ---------------------------------------------------------------------------
void loop() {
  unsigned long currentMillis = millis();

  // ---------------------------------------------------------
  // 1. Check for Python Commands (Priority 1)
  // ---------------------------------------------------------
  WiFiClient pythonClient = server.available();
  
  if (pythonClient) {
    if (pythonClient.connected() && pythonClient.available()) {
      char command = pythonClient.read();
      
      Serial.print("Received Command: ");
      Serial.println(command);
      
      if (command == '1') {
        RotateMotors('r'); // Trash
      } 
      else if (command == '0') {
        RotateMotors('l'); // Recyclable
      }
    }
    pythonClient.stop(); // Close connection immediately
  }

  // ---------------------------------------------------------
  // 2. Check for Serial Commands (Manual Override)
  // ---------------------------------------------------------
  if(Serial.available() > 0){
    char ch = Serial.read();
    RotateMotors(ch);

    // If a command was received, wait then read sensors immediately
    if (ch) {
      delay(3000); 
      
      float currentDistance = getFilteredDistance(TRIG_PIN, ECHO_PIN);
      printDistance(currentDistance);
      
      float currentDistance2 = getFilteredDistance(TRIG_PIN2, ECHO_PIN2);
      printDistance(currentDistance2);
      
      correctDistance = compareSensors(currentDistance, currentDistance2);
    }
  }

  NeoPixel.clear();

  // ---------------------------------------------------------
  // 3. AUTOMATIC SENSOR TIMER (Runs every 10 seconds)
  // ---------------------------------------------------------
  // Note: We removed "&& !ch" because 'ch' only exists inside the block above.
  // The delay(3000) above naturally prevents this from overlapping.
  
  if (currentMillis - lastSensorTimeSensors >= sensorIntervalSensors) {
    // Save the last time we ran this code
    lastSensorTimeSensors = currentMillis; 

    Serial.println("--- 10 Second Interval: Reading Sensors ---");

    // Read Sensors
    float d1 = getFilteredDistance(TRIG_PIN, ECHO_PIN);
    printDistance(d1);

    float d2 = getFilteredDistance(TRIG_PIN2, ECHO_PIN2);
    printDistance(d2);

    // Compare and update global variable
    correctDistance = compareSensors(d1, d2);
  } // <--- End of Timer Block


  // ---------------------------------------------------------
  //  Update Display & Cloud
  // ---------------------------------------------------------
  display_trash_percent(correctDistance);
}