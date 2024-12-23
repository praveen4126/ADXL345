#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

// Thresholds for movement detection
const int X_THRESHOLD = 200;  // Tune these thresholds
const int Y_THRESHOLD = 200;

// Sequence-related variables
int expectedSequence[] = {1, 2, 3, 4, 5};
int currentStep = 0;

// Flags for movement detection
int lastMove = 0;

void setup() {
  Serial.begin(115200);

  if (!accel.begin()) {
    Serial.println("ADXL345 not detected!");
    while (1);
  }

  accel.setRange(ADXL345_RANGE_2_G);
}

void loop() {
  sensors_event_t event;
  accel.getEvent(&event);

  int x = event.acceleration.x * 100;  // Scale acceleration for easier comparison
  int y = event.acceleration.y * 100;

  int detectedMove = detectMovement(x, y);

  if (detectedMove > 0) {
    if (detectedMove == expectedSequence[currentStep]) {
      // Correct sequence detected
      if (detectedMove != lastMove) {
        Serial.print("Correct Sequence Detected: ");
        Serial.println(detectedMove);
        lastMove = detectedMove;
      }
      currentStep++;
      if (currentStep == 5) {
        Serial.println("Sequence Complete!");
        currentStep = 0;  // Reset for the next sequence
      }
    }
  }

  delay(100);  // Debounce delay
}

// Function to detect movement based on X and Y thresholds
int detectMovement(int x, int y) {
  if (x > X_THRESHOLD) return 1;  // Right
  if (x < -X_THRESHOLD) return 2; // Left
  if (y > Y_THRESHOLD) return 3;  // Forward
  if (y < -Y_THRESHOLD) return 4; // Backward
  if (abs(x) < X_THRESHOLD && abs(y) < Y_THRESHOLD) return 5;  // Stationary
  return 0;  // No movement detected
}
