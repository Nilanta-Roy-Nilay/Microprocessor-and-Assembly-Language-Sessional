// আলট্রাসনিক সেন্সরের পিনগুলি
const int trigPin = 11;
const int echoPin = 12;

// LED পিনগুলি (আপনার সার্কিট অনুযায়ী পিন নম্বরগুলি পরিবর্তন করুন)
const int ledPin1 = 5; // প্রথম LED (সবুজ, ধরুন)
const int ledPin2 = 6; // দ্বিতীয় LED (কমলা)
const int ledPin3 = 7; // তৃতীয় LED (কমলা)
const int ledPin4 = 8; // চতুর্থ LED (লাল)

// দূরত্বের ভেরিয়েবল
long duration;
int distanceCm;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // LED পিনগুলিকে আউটপুট হিসাবে সেট করুন
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);

  // শুরুতে সব LED বন্ধ করুন
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);
  digitalWrite(ledPin4, LOW);

  Serial.begin(9600); // দূরত্বের মান দেখতে
}

void loop() {
  // আলট্রাসনিক সেন্সর থেকে দূরত্ব পরিমাপ করুন
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * 0.034 / 2; // দূরত্ব সেন্টিমিটারে

  Serial.print("Distance: ");
  Serial.print(distanceCm);
  Serial.println(" cm");

  // --- LED নিয়ন্ত্রণ লজিক ---
  
  // শুরুতে সব LED বন্ধ করুন
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);
  digitalWrite(ledPin4, LOW);

  // দূরত্বের উপর ভিত্তি করে LED জ্বালা/নেভানো
  if (distanceCm < 10) {
    // খুব কাছে (10 cm-এর কম) - লাল LED জ্বালাবে
    digitalWrite(ledPin4, HIGH); 
  } else if (distanceCm >= 10 && distanceCm < 20) {
    // কাছাকাছি (10 থেকে 20 cm) - কমলা LED জ্বালাবে
    digitalWrite(ledPin3, HIGH);
  } else if (distanceCm >= 20 && distanceCm < 30) {
    // মাঝারি দূরত্ব (20 থেকে 30 cm) - অন্য কমলা LED জ্বালাবে
    digitalWrite(ledPin2, HIGH);
  } else {
    // দূরে (30 cm-এর বেশি) - সবুজ LED জ্বালাবে
    digitalWrite(ledPin1, HIGH);
  }

  delay(500); // 0.5 সেকেন্ড অপেক্ষা করুন
}