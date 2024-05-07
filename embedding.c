#include <SoftwareSerial.h>
#define sos D7
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#define buzzer D0
#define led D4
#define REPORTING_PERIOD_MS     1000

// Create a PulseOximeter object
PulseOximeter pox;

// Time at which the last beat occurred
uint32_t tsLastReport = 0;

// Callback routine is executed when a pulse is detected
void onBeatDetected() {
  Serial.println("♥ Beat!");
}
SoftwareSerial mySerial(D5, D6);
void SendMessage();
void setup()
{
  mySerial.begin(9600); // Setting the baud rate of GSM Module
  Serial.begin(9600); // Setting the baud rate of Serial Monitor (Arduino)
  Serial.println("gsm started");
  delay(100);
  pinMode(sos, INPUT_PULLUP);pinMode(buzzer, OUTPUT);pinMode(led, OUTPUT);
  Serial.print("Initializing pulse oximeter..");

  // Initialize sensor
  if (!pox.begin()) {
    Serial.println("FAILED");
    for (;;);
  } else {
    Serial.println("SUCCESS");
  }

  // Configure sensor to use 7.6mA for LED drive
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

  // Register a callback routine
  pox.setOnBeatDetectedCallback(onBeatDetected);

}

void loop()
{
  digitalWrite(buzzer, LOW);
  digitalWrite(led, LOW);
  // Read from the sensor
  pox.update();

  // Grab the updated heart rate and SpO2 levels
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    Serial.print("Heart rate:");
    Serial.print(pox.getHeartRate());
    Serial.print("bpm / SpO2:");
    Serial.print(pox.getSpO2());
    Serial.println("%");

    tsLastReport = millis();
  }

  if (digitalRead(sos) == 0) {
    SendMessage();
  }

  if (mySerial.available() > 0)
  {
    Serial.write(mySerial.read());
  }
}

void SendMessage()
{
  mySerial.println("AT+CMGF=1"); // Sets the GSM Module in Text Mode
  delay(1000); // Delay of 1000 milliseconds or 1 second

  mySerial.println("AT+CMGS=\"+918368206200\"\r"); // Replace x with mobile number
  delay(1000);

  mySerial.println("BPM is "); // The SMS text you want to send
    mySerial.println(pox.getHeartRate()); // The SMS text you want to send
     mySerial.println("SPO2 is "); // The SMS text you want to send
    mySerial.println(pox.getSpO2()); // The SMS text you want to send

mySerial.println("latitude is" ); // The SMS text you want to send
mySerial.println(serial.get_latitude()); // The SMS text you want to send
mySerial.println("longitude is"); // The SMS text you want to send
mySerial.println(serial.get_longitude()); // The SMS text you want to send

  delay(100);

  mySerial.println((char)26); // ASCII code of CTRL+Z
  digitalWrite(buzzer, HIGH);
  digitalWrite(led, HIGH);
  delay(1000);
  
}