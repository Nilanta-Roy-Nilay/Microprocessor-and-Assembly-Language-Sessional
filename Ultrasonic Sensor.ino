// Pin definitions (Ei pin gulo apnar connection onujayi adjust korun)
const int trigPin = 9;   // Ultrasonic Sensor Trigger Pin
const int echoPin = 10;  // Ultrasonic Sensor Echo Pin
const int greenLedPin = 11; // Shobuj LED Pin (Dhore neowa holo)
const int redLedPin = 12;   // Lal LED Pin (Dhore neowa holo)

// Variables
long duration; // Duration of the sound wave travel
int distanceCm; // Distance in centimeters

// Threshold for turning on the Green LED (e.g., within 30 cm)
const int distanceThresholdCm = 30; // 30 cm dori

void setup() {
  // Initialize the pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT); // Lal LED o set kora holo, dorkar na holeo
  
  // Initialize Serial communication for debugging (optional in Tinkercad, but helpful)
  Serial.begin(9600); 
}

void loop() {
  // Clear the trigPin for a moment
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Set the trigPin high for 10 microseconds to send the pulse
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the duration of the echo signal
  duration = pulseIn(echoPin, HIGH);

  // Convert the time into a distance
  // Distance = (Duration * Speed of Sound (0.0343 cm/µs)) / 2
  // For common use, this formula is used for cm: 
  distanceCm = duration * 0.034 / 2;

  // Print the distance for debugging
  Serial.print("Distance: ");
  Serial.print(distanceCm);
  Serial.println(" cm");

  // Logic to control the LEDs
  if (distanceCm <= distanceThresholdCm && distanceCm > 0) { 
    // If the object is close (within 30 cm) and distance is valid
    digitalWrite(greenLedPin, HIGH); // Turn ON Green LED
    digitalWrite(redLedPin, LOW);    // Turn OFF Red LED
  } else {
    // If the object is far or the sensor reading is invalid
    digitalWrite(greenLedPin, LOW);  // Turn OFF Green LED
    digitalWrite(redLedPin, HIGH);   // Turn ON Red LED (optional)
  }

  delay(500); // Wait for half a second before the next reading
}