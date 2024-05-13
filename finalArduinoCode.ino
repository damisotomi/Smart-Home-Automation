#include <Arduino.h>
#include <Servo.h>
#include <SoftwareSerial.h>

// Setup servo object
Servo myServo;

// Define pin assignments
const int ledPin = 2;
const int motorDriverPin1 = 3;
const int motorDriverPin2 = 4;
const int motorDriverPin3 = 5;
const int motorDriverPin4 = 6;
const int servoPin = 9;
const int rxPin = 11;  // RX pin for LCD (not used)
const int txPin = 10;  // TX pin for LCD

// Initialize the SoftwareSerial for LCD
SoftwareSerial lcd(txPin, rxPin);

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  lcd.begin(9600); // Initialize LCD communication
  
  // Initialize pin modes
  pinMode(ledPin, OUTPUT);
  pinMode(motorDriverPin1, OUTPUT);
  pinMode(motorDriverPin2, OUTPUT);
  pinMode(motorDriverPin3, OUTPUT);
  pinMode(motorDriverPin4, OUTPUT);
  myServo.attach(servoPin);  // Attach servo motor to its control pin

  // Ensure all devices are off initially
  digitalWrite(ledPin, LOW);
  digitalWrite(motorDriverPin1, LOW);
  digitalWrite(motorDriverPin2, LOW);
  digitalWrite(motorDriverPin3, LOW);
  digitalWrite(motorDriverPin4, LOW);
  myServo.write(50);  // Set servo position to 0 degrees

  // Clear LCD and setup initial display
  lcd.write(17);
  lcd.write(12); // Clear screen command
  lcd.print("Welcome Home!");
  delay(3000);
  lcd.write(12);
  delay(10); // Short delay
  lcd.print("L    F   W    D "); // Assuming W is for Water Pump
}

void controlDevices(int binaryValue) {
  // Update LCD with the state of each device
  lcd.write(12); // Clear screen
  delay(10); // Short delay for LCD to process clear command
  lcd.print("L    F   W    D ");
  lcd.print((binaryValue & 0x01) ? "ON  " : "OFF ");
  lcd.print(((binaryValue >> 1) & 0x01) ? "ON   " : "OFF ");
  lcd.print(((binaryValue >> 2) & 0x01) ? "ON " : "OFF ");
  lcd.print(((binaryValue >> 3) & 0x01) ? "ON" : " OFF");
  
  // Control LED
  digitalWrite(ledPin, binaryValue & 0x01);

  // Control DC Motor
  digitalWrite(motorDriverPin1, (binaryValue >> 1) & 0x01);
  digitalWrite(motorDriverPin2, LOW); 

  // Control Water Pump
  digitalWrite(motorDriverPin3, (binaryValue >> 2) & 0x01);
  digitalWrite(motorDriverPin4, LOW);

  // Control Servo Motor
  if ((binaryValue >> 3) & 0x01)
    myServo.write(170);  // Move servo to 180 degrees
  else
    myServo.write(50);    // Move servo back to 0 degrees
}

void loop() {
  if (Serial.available() > 0) {
    int detectedValue = Serial.parseInt();  // Read the incoming integer
    controlDevices(detectedValue);  // Control devices based on the binary input
  }
}