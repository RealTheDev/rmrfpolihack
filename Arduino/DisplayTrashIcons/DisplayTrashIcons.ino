#include "Arduino_LED_Matrix.h"
#include "trash_icons.h" 

ArduinoLEDMatrix matrix;

void setup() {
  Serial.begin(115200);
  matrix.begin();
  
  // Prompt the user once at startup
  Serial.println("System Ready."); 
  Serial.println("Type a percentage (0.0, 12.5, 25.0...) and press Enter:");
}

// Helper to check if two floats are approximately equal
bool isClose(float a, float b) {
  return abs(a - b) < 0.1; // Checks if they are within 0.1 of each other
}

void display_trash_icons_float(float respons_percent) {
  
  // 1. Logic for >= 100%
  if (respons_percent >= 100.0) {
    matrix.loadFrame(Trash_can_100);
    Serial.println("Displaying: 100%");
    
  } else if (respons_percent >= 87.5) {
    matrix.loadFrame(Trash_can_87_5);
    Serial.println("Displaying: 87.5%");
    
  } else if (respons_percent >= 75.0) {
    matrix.loadFrame(Trash_can_75);
    Serial.println("Displaying: 75%");
    
  } else if (respons_percent >= 65.5) { 
    // Note: Standard 5/8ths is usually 62.5, but I kept your 65.5 threshold
    // Ensure your variable in trash_icons.h matches this name (likely Trash_can_62_5)
    matrix.loadFrame(Trash_can_62_5); 
    Serial.println("Displaying: 62.5% (Threshold 65.5)");
    
  } else if (respons_percent >= 50.0) {
    matrix.loadFrame(Trash_can_50);
    Serial.println("Displaying: 50%");
    
  } else if (respons_percent >= 37.5) {
    matrix.loadFrame(Trash_can_37_5);
    Serial.println("Displaying: 37.5%");
    
  } else if (respons_percent >= 25.0) {
    matrix.loadFrame(Trash_can_25);
    Serial.println("Displaying: 25%");
    
  } else if (respons_percent >= 12.5) {
    matrix.loadFrame(Trash_can_12_5);
    Serial.println("Displaying: 12.5%");
    
  } else if (respons_percent >= 0.0) {
    matrix.loadFrame(Trash_can_0);
    Serial.println("Displaying: 0%");
    
  } else {
    // Default / Error case (negative numbers)
    Serial.print("Unknown value: ");
    Serial.println(respons_percent);
  }
}

void loop() {
  // 1. Check if data is available in the Serial buffer
  if (Serial.available() > 0) {
    
    // 2. Read the incoming float
    float user_input = Serial.parseFloat();
    
    // 3. Clear the "New Line" character from the buffer so it doesn't read 0.0 next time
    while (Serial.available() > 0) {
      Serial.read(); 
    }

    // 4. Provide feedback to the user
    Serial.print("Received: ");
    Serial.println(user_input);

    // 5. Update the Display
    display_trash_icons_float(user_input);
  }
}