#include <Servo.h> // সার্ভো মোটরের জন্য প্রয়োজনীয় লাইব্রেরি

/*
  "Sizzling Jaagub" প্রজেক্টের কোড
  
  কম্পোনেন্ট এবং পিন সংযোগ:
  - PIR Motion Sensor (সিগন্যাল) -> Digital Pin 7
  - Servo Motor (সিগন্যাল)     -> Digital Pin 9 (PWM ~)
  - Piezo Buzzer (পজিটিভ)       -> Digital Pin 11 (PWM ~)
  - LED (পজিটিভ/Anode)         -> Digital Pin 13
*/

// --- পিন নম্বরগুলো ডিফাইন করা ---
const int pirPin = 7;
const int servoPin = 9;
const int ledPin = 13;
const int buzzerPin = 11;

// --- ভ্যারিয়েবল ---
Servo myServo;             // সার্ভো অবজেক্ট তৈরি করা
int motionState = 0;       // PIR সেন্সরের বর্তমান অবস্থা (0 = no motion, 1 = motion)

void setup() {
  // --- পিন মোড সেট করা ---
  pinMode(pirPin, INPUT);   // PIR পিন একটি ইনপুট
  pinMode(ledPin, OUTPUT);  // LED পিন একটি আউটপুট
  pinMode(buzzerPin, OUTPUT); // Buzzer পিন একটি আউটপুট

  // --- সার্ভো মোটর চালু করা ---
  myServo.attach(servoPin); // ৯ নম্বর পিনে সার্ভোকে যুক্ত করা
  myServo.write(0);         // শুরুতে সার্ভোকে ০ ডিগ্রিতে রাখা
  
  // ডিবাগিং এর জন্য সিরিয়াল মনিটর (ঐচ্ছিক)
  Serial.begin(9600);
}

void loop() {
  // --- PIR সেন্সর থেকে ডেটা রিড করা ---
  motionState = digitalRead(pirPin);

  // --- চেক করা: মোশন বা নড়াচড়া শনাক্ত হয়েছে কিনা ---
  if (motionState == HIGH) {
    // --- যদি মোশন শনাক্ত হয় ---
    
    digitalWrite(ledPin, HIGH);   // LED জ্বালানো
    tone(buzzerPin, 500);       // Buzzer-এ ৫০০ হার্জ ফ্রিকোয়েন্সিতে শব্দ করা
    myServo.write(90);          // সার্ভো মোটরকে ৯০ ডিগ্রিতে ঘোরানো
    
    Serial.println("Motion Detected!"); // সিরিয়াল মনিটরে প্রিন্ট করা
  } 
  else {
    // --- যদি কোনো মোশন না থাকে ---
    
    digitalWrite(ledPin, LOW);    // LED নেভানো
    noTone(buzzerPin);          // Buzzer-এর শব্দ বন্ধ করা
    myServo.write(0);           // সার্ভো মোটরকে আবার ০ ডিগ্রিতে ফেরত আনা
    
    Serial.println("No Motion."); // সিরিয়াল মনিটরে প্রিন্ট করা
  }

  delay(100); // প্রতি চক্রের মাঝে সামান্য দেরি (সেন্সরের জন্য ভালো)
}