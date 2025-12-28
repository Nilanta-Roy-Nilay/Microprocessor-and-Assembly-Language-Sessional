#include <Keypad.h>
#include <LiquidCrystal_I2C.h> // I2C LCD এর জন্য

// --- পিন কনফিগারেশন (Pin Definitions) ---
// Keypad এর পিন (আপনার ছবি অনুযায়ী 4x4 keypad)
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
// আপনার ছবি অনুযায়ী পিনগুলি সঠিকভাবে যুক্ত করুন
byte rowPins[ROWS] = {9, 8, 7, 6}; // Arduino পিন রো এর জন্য
byte colPins[COLS] = {5, 4, 3, 2}; // Arduino পিন কলাম এর জন্য

// সেন্সর ও অ্যাকচুয়েটরের পিন
const int pirPin = 10;        // PIR সেন্সরের আউটপুট পিন
const int lm35Pin = A0;       // LM35 টেম্পারেচার সেন্সরের এনালগ পিন
const int ledPin = 11;        // সোলেনয়েড/LED এর পিন (আপনার ছবিতে LED দেখা যাচ্ছে, তাই নাম পরিবর্তন করা হলো)
const int buzzerPin = 12;     // বাজার (Buzzer) পিন

// --- LCD সেটআপ (LCD Setup) ---
// I2C LCD এর ঠিকানা (Address) এবং সাইজ (16x2)
LiquidCrystal_I2C lcd(0x27, 16, 2); 

// --- Keypad অবজেক্ট তৈরি (Keypad Object) ---
Keypad customKeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// --- ভ্যারিয়েবল (Variables) ---
String correctPass = "1234";  // আপনার সেট করা পাসওয়ার্ড
String inputCode = "";
bool isArmed = false;       // সিস্টেম আর্মড আছে কিনা

// ------------------------------------
// Setup ফাংশন
// ------------------------------------
void setup() {
  Serial.begin(9600);
  
  // পিন মোড সেট করা
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT); // LED/সোলেনয়েড পিন
  pinMode(buzzerPin, OUTPUT);
  
  // LED বন্ধ (বা সোলেনয়েড লক)
  digitalWrite(ledPin, LOW); 
  digitalWrite(buzzerPin, LOW);
  
  // LCD ইনিশিয়ালাইজ করা
  lcd.init();
  lcd.backlight();
  lcd.clear();

  // ***** এখানে প্রথমবার আপনার চাওয়া মেসেজটি ডিসপ্লে করা হচ্ছে *****
  
  // একটি কাল্পনিক মান 74.8 সেট করা হলো, যেহেতু সিমুলেটরে ঠিক এই মানটি চান
  float initialTemp = 74.8;
  
  lcd.setCursor(0, 0);
  lcd.print("No Motion"); // প্রথম লাইন
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(initialTemp, 1); // 74.8
  lcd.print((char)223); // ডিগ্রি চিহ্নের জন্য
  lcd.print("C");
  
  // ***** LED/বাল্ব চালু করা হচ্ছে *****
  digitalWrite(ledPin, HIGH); 
  
  // ** বিঃদ্রঃ: সিমুলেশনে LM35 এর এনালগ রিডিং সাধারণত 74.8 ডিগ্রি C দেয় না। 
  // এই ফাংশনটি শুধু প্রথম ডিসপ্লের জন্য। লুপে আসল তাপমাত্রা রিড করা হবে।
  
  delay(5000); // এই মেসেজটি ৫ সেকেন্ড ধরে দেখাবে
  lcd.clear(); // তারপর লুপের কাজ শুরু হবে
}

// ------------------------------------
// Loop ফাংশন
// ------------------------------------
void loop() {
  // Keypad ইনপুট চেক করা
  char key = customKeypad.getKey();
  if (key) {
    handleKey(key);
  }

  // তাপমাত্রা পরিমাপ ও ডিসপ্লে
  float temperature = readTemperature();
  lcd.setCursor(0, 0);
  
  // PIR সেন্সর চেক করা (যদি সিস্টেম আর্মড থাকে)
  int motion = digitalRead(pirPin);
  
  if (motion == HIGH) {
      lcd.print("Motion Detected!");
      digitalWrite(ledPin, LOW); // মোশন থাকলে LED বন্ধ 
    if (isArmed) {
      triggerAlarm();
    }
  } else {
      lcd.print("No Motion       "); // 16 অক্ষরের স্পেস দিয়ে ওভাররাইড
      if (!isArmed) {
        digitalWrite(ledPin, HIGH); // ডিসআর্মড এবং মোশন না থাকলে LED জ্বলবে (যেমনটি ছবিতে আছে)
      } else {
        digitalWrite(ledPin, LOW); // আর্মড থাকলে LED বন্ধ
      }
  }

  // দ্বিতীয় লাইন: তাপমাত্রা এবং আর্মড স্ট্যাটাস
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(temperature, 1); // এক দশমিক স্থান পর্যন্ত
  lcd.print((char)223); // ডিগ্রি চিহ্নের জন্য
  lcd.print("C");
  lcd.print(isArmed ? " ARMED" : " DISARMED"); // স্ট্যাটাস দেখাবে
  
  delay(100); // একটি ছোট বিলম্ব
}

// ------------------------------------
// অতিরিক্ত ফাংশন (পূর্বের কোড অনুযায়ী)
// ------------------------------------

void handleKey(char key) {
  // ... (পূর্বের handleKey ফাংশন) ...
  if (key == '#') {
    if (inputCode == correctPass) {
      if (isArmed) {
        disarmSystem();
      } else {
        armSystem();
      }
    } else {
      showError("Wrong PIN!");
      inputCode = "";
    }
  } else if (key == '*') {
    inputCode = "";
    showError("PIN Cleared");
  } else {
    inputCode += key;
    lcd.setCursor(10, 0); 
    lcd.print(inputCode);
  }
}

// তাপমাত্রা পরিমাপ ফাংশন
float readTemperature() {
  int analogValue = analogRead(lm35Pin);
  float voltage = analogValue * (5.0 / 1024.0);
  float tempC = voltage * 100.0;
  return tempC;
}

// সিস্টেম আর্ম করা
void armSystem() {
  isArmed = true;
  digitalWrite(ledPin, LOW); // আর্মড হলে LED বন্ধ
  digitalWrite(buzzerPin, LOW);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System ARMED!");
  delay(2000);
  inputCode = "";
  lcd.clear();
}

// সিস্টেম ডিসআর্ম করা
void disarmSystem() {
  isArmed = false;
  digitalWrite(ledPin, HIGH); // ডিসআর্মড হলে LED চালু
  digitalWrite(buzzerPin, LOW);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System DISARMED");
  delay(2000);
  inputCode = "";
  lcd.clear();
}

// অ্যালার্ম বাজানো
void triggerAlarm() {
  // ... (পূর্বের triggerAlarm ফাংশন) ...
  for(int i = 0; i < 20; i++) { 
    digitalWrite(buzzerPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
    delay(100);
  }
}

// এরর ডিসপ্লে করা
void showError(String msg) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(msg);
  delay(1500);
  lcd.clear();
}
