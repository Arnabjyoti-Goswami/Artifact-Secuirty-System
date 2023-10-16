#include <Adafruit_LiquidCrystal.h>

// Setup the threshold values for the security system:
const int minTemp = 20; // 20 degree celcius is set as the threshold
const int minDistance = 100; // 100cm is the threshold
const int maxSmoke = 500; // A value of 500 read from the smoke sensor is set as the threshold

// Setup the pins to variable names so that we don't need to remember their values:
const int trigPin = 7; // Hypersonic sensor trigger pin
const int echoPin = 6; // Hypersonic sensor echo pin

const int smokePin = A1; // Smoke sensor analog pin
const int buzzerPin = 9; // Buzzer digital pin
const int temperaturePin = A0; // Temperature sensor analog pin

const int ledPinTemperature = 12; // LED pin for low temperature detection
const int ledPinIntrusion = 13; // LED pin for intrusion indication

Adafruit_LiquidCrystal lcd_1(0);

void setup()
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(smokePin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPinTemperature, OUTPUT);
  pinMode(ledPinIntrusion, OUTPUT);

  lcd_1.begin(16, 2);
  
  Serial.begin(9600);
}

void loop()
{
  //  Trigger the HC-SR04 ultrasonic sensor to send out an ultrasonic pulse for distance measurement. 
  // The sensor sends out an ultrasonic pulse when the trigger pin goes from LOW to HIGH and stops sending the pulse when the trigger pin returns to LOW.
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(10);
  
  // Set all to detectors to low at the beginning of each loop (this is the default condition of the room):
  digitalWrite(buzzerPin, LOW); // Turn off buzzer
  digitalWrite(ledPinIntrusion, LOW); // Turn off intrusion LED
  digitalWrite(ledPinTemperature, LOW); // Turn off temperature LED

  // Find distance from ultrasonic distance sensor:
  long duration = pulseIn(echoPin, HIGH);
  int distance = duration / 58.2;
  
  // Read smoke sensor value:
  int smokeStatus = analogRead(smokePin);
  
  // Measure temperature in Celsius:
  int temperature = map(((analogRead(A0) - 20) * 3.04), 0, 1023, -40, 125); // Read the temperature sensor analog value and map it into celcius scale
  
  if(!(smokeStatus > maxSmoke) && !(distance < minDistance) && !(temperature < minTemp)) {
    // Display in LCD:
    lcd_1.setCursor(0, 0); // 0-th column, 0-th row (16x2 LCD display)
    lcd_1.print("Room Status:    ");
    lcd_1.setCursor(0, 1); // 0-th column, 1-st row (16x2 LCD display)
    lcd_1.print("Safe           ");
  }
  else if( ((smokeStatus > maxSmoke) && (distance < minDistance) && (temperature < minTemp)) || (!(smokeStatus > maxSmoke) && (distance < minDistance) && (temperature < minTemp))) {
    //the smoke sensor not working and other 2 sensors working OR all 3 working at the same time is the same condition logically, as LED for smoke detection was not told to implement in problem statement

    // Display in LCD:
    lcd_1.setCursor(0, 0);
    lcd_1.print("Compromised and");
    lcd_1.setCursor(0, 1);
    lcd_1.print("Heating Room");
    
    digitalWrite(ledPinIntrusion, HIGH); // Turn on intrusion LED as distance is less than 100cm
    digitalWrite(ledPinTemperature, HIGH); // Turn on temperature LED as temperature is less than 30 degree celcius

    // Turn on the buzzer:
    digitalWrite(buzzerPin, HIGH); // Turn on the buzzer
    tone(buzzerPin, 500, 500); // Send 500 Hz sound signal to play for 500ms
    delay(500); // Delay the void loop() function for 500ms so that the buzzer can actually ring for 0.5 seconds
  }
  else if((smokeStatus > maxSmoke) && !(distance < minDistance) && !(temperature < minTemp)) {
    // Display in LCD:
    lcd_1.setCursor(0, 0);
    lcd_1.print("Room Status:    ");
    lcd_1.setCursor(0, 1);
    lcd_1.print("Compromised  ");

    // Turn on the buzzer:
    digitalWrite(buzzerPin, HIGH);
    tone(buzzerPin, 500, 500);
    delay(500);
  }
  else if(!(smokeStatus > maxSmoke) && (distance < minDistance) && !(temperature < minTemp)) {
    // Display in LCD:
    lcd_1.setCursor(0, 0);
    lcd_1.print("Room Status:    ");
    lcd_1.setCursor(0, 1);
    lcd_1.print("Compromised  ");
    
    digitalWrite(ledPinIntrusion, HIGH); // Turn on intrusion LED as distance is less than 100cm

    // Turn on the buzzer:
    digitalWrite(buzzerPin, HIGH);
    tone(buzzerPin, 500, 500);
    delay(500);
  }
  else if(!(smokeStatus > maxSmoke) && !(distance < minDistance) && (temperature < minTemp)) {
    // Display in LCD:
    lcd_1.setCursor(0, 0);
    lcd_1.print("Room Status:    ");
    lcd_1.setCursor(0, 1);
    lcd_1.print("Heating Room");

    digitalWrite(ledPinTemperature, HIGH); // Turn on temperature LED as temperature is less than 30 degree celcius
    delay(500); // so that LED can glow for .5s
  }
  else if((smokeStatus > maxSmoke) && (distance < minDistance) && !(temperature < minTemp)) {
    // Display in LCD:
    lcd_1.setCursor(0, 0);
    lcd_1.print("Room Status:    ");
    lcd_1.setCursor(0, 1);
    lcd_1.print("Compromised  ");
    
    digitalWrite(ledPinIntrusion, HIGH); // Turn on intrusion LED as distance is less than 100cm

    // Turn on the buzzer:
    digitalWrite(buzzerPin, HIGH);
    tone(buzzerPin, 500, 500);
    delay(500);
  }
  else if((smokeStatus > maxSmoke) && !(distance < minDistance) && (temperature < minTemp)) {
    // Display in LCD:
    lcd_1.setCursor(0, 0);
    lcd_1.print("Compromised and");
    lcd_1.setCursor(0, 1);
    lcd_1.print("Heating Room");
    
    digitalWrite(ledPinTemperature, HIGH); // Turn on temperature LED as temperature is less than 30 degree celcius

    // Turn on the buzzer:
    digitalWrite(buzzerPin, HIGH);
    tone(buzzerPin, 500, 500);
    delay(500);
  }
}