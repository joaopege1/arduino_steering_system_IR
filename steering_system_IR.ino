#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <IRremote.hpp>

// LCD and Servo Configuration
LiquidCrystal_I2C lcd(0x27, 16, 2); 
Servo myServo;

// Pin Configuration
const int RECV_PIN = 4;  
const int servoPin = 9;  
const int leftLed = 7; 
const int rightLed = 2;  

// Remote Control Hex Codes
const uint16_t BUTTON_RIGHT   = 0x8; 
const uint16_t BUTTON_LEFT    = 0x5A;
const uint16_t BUTTON_FORWARD = 0x18;
const uint16_t BUTTON_BACK    = 0x52;

// Control Variables
int angle = 90;            // Starts at 90 (Straight)
const int stepSize = 180;  // Goes to the extreme with 1 click

unsigned long previousTime = 0;
const int blinkInterval = 300;

void setup() {
  Serial.begin(9600);
  lcd.init();          
  lcd.backlight();    
  
  myServo.attach(servoPin);
  
  pinMode(leftLed, OUTPUT); 
  pinMode(rightLed, OUTPUT);
  
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);
  
  myServo.write(angle); 
  
  lcd.setCursor(0, 0);
  lcd.print("Johnny The Maker");
}

void loop() {
  
  // ==========================================
  // 1. IR REMOTE READING
  // ==========================================
  if (IrReceiver.decode()) {
    uint16_t command = IrReceiver.decodedIRData.command;
    
    Serial.print("Command received: 0x");
    Serial.println(command, HEX);

    if (command == BUTTON_RIGHT) {
      angle -= stepSize; // Goes to 0
    } 
    else if (command == BUTTON_LEFT) {
      angle += stepSize; // Goes to 180
    }
    else if (command == BUTTON_FORWARD || command == BUTTON_BACK) {
      angle = 90;        // Sets to 90 degrees (Straight)
    }
    
    // Ensures the value stays between 0 and 180
    angle = constrain(angle, 0, 180);
    
    myServo.write(angle);
    IrReceiver.resume(); 
  }

  // ==========================================
  // 2. LCD UPDATE
  // ==========================================
  lcd.setCursor(0, 1);
  lcd.print("Angle: ");
  lcd.print(angle);
  lcd.print("   "); 

  // ==========================================
  // 3. LED LOGIC (Non-blocking)
  // ==========================================
  unsigned long currentTime = millis();
  if (currentTime - previousTime >= blinkInterval) {
    previousTime = currentTime;
    
    // If turned left (Low angle)
    if (angle < 84) {
      digitalWrite(rightLed, !digitalRead(rightLed)); // Blinks the left LED
      digitalWrite(leftLed, HIGH);                    // Keeps the other one steady
    } 
    // If turned right (High angle)
    else if (angle > 95) {
      digitalWrite(leftLed, !digitalRead(leftLed));   // Blinks the right LED
      digitalWrite(rightLed, HIGH);
    } 
    // If straight (90 degrees)
    else {
      digitalWrite(leftLed, HIGH);
      digitalWrite(rightLed, HIGH);
    }
  }
}
