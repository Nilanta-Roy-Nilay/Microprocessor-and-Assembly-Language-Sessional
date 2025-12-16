// Pin Definitions
const int sensorPin = A0;      // Analog pin jekhane LDR/Phototransistor connect kora ache
const int ledPin = 13;         // Digital pin jekhane LED connect kora ache (chobi onujayi dhora holo)

// Threshold Value
// Ei maan LDR er value-r upor nirbhor kore, jeta alo/andhokar-e change hoy
// Apnar LDR set-up-e andhokar hole value beshi hobe (jodi voltage divider-er configuration thik thake).
// Apni Tinkercad-e Serial Monitor use kore ashol maan peye adjust korte paren.
const int lightThreshold = 500; 

// Variables
int sensorValue = 0; // Sensor theke pora analog maan rakhar jonno

void setup() {
  // LED pin ke output hishebe set kora holo
  pinMode(ledPin, OUTPUT);
  
  // Serial communication start kora holo debugging er jonno
  Serial.begin(9600);
}

void loop() {
  // Sensor pin theke analog value pora holo (0 theke 1023 er modhye)
  sensorValue = analogRead(sensorPin);

  // Serial Monitor-e maan print kora holo
  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);

  // Logic: Jodi sensor value threshold er theke beshi hoy (kom alo/andhokar), tahole LED jole uthbe
  if (sensorValue > lightThreshold) {
    // LED ON
    digitalWrite(ledPin, HIGH);
    Serial.println("-> Low Light Detected: LED ON");
  } else {
    // LED OFF
    digitalWrite(ledPin, LOW);
    Serial.println("-> Bright Light Detected: LED OFF");
  }

  // 100 milliseconds wait kora holo poroborti reading er jonno
  delay(100); 
}