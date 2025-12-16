// এই কোডটি গ্যাস/ফায়ার অ্যালার্ম সিস্টেমের জন্য তৈরি করা হয়েছে, যেখানে Gas Sensor, 8টি LED, Buzzer এবং I2C 16x2 LCD ডিসপ্লে ব্যবহার করা হয়েছে।
// এটি ৫ বা তার বেশি LED জ্বললে ফায়ার অ্যালার্ম ট্রিগার করবে এবং LCD-তে বার্তা প্রদর্শন করবে।

// --- LIBRARIES FOR LCD (LCD-এর জন্য প্রয়োজনীয় লাইব্রেরি) ---
#include <Wire.h>              // I2C যোগাযোগের জন্য প্রয়োজন
#include <LiquidCrystal_I2C.h> // I2C LCD নিয়ন্ত্রণের জন্য প্রয়োজন

// --- PIN DEFINITIONS (পিন সংজ্ঞা) ---
const int GAS_SENSOR_PIN = A0;   // গ্যাস সেন্সরের অ্যানালগ আউটপুট (A0) পিন
const int BUZZER_PIN = 10;       // বুজার/পিজো স্পিকারের ডিজিটাল পিন

// 8টি LED (আপনার 'বাল্ব') এর ডিজিটাল পিনগুলির অ্যারে
const int LED_PINS[] = {2, 3, 4, 5, 6, 7, 8, 9};
const int NUM_LEDS = 8;          // মোট LED সংখ্যা

// --- LCD SETUP (LCD সেটআপ) ---
// I2C ঠিকানা এবং LCD-এর মাত্রা সংজ্ঞায়িত করুন। সাধারণত 0x27 ব্যবহার করা হয়।
LiquidCrystal_I2C lcd(0x27, 16, 2);

// --- THRESHOLD CONSTANTS (থ্রেশহোল্ড কনস্ট্যান্টস) ---
const int GAS_MIN_READING = 200; // 'পরিষ্কার বাতাস' এর জন্য অ্যানালগ মান
const int GAS_MAX_READING = 800; // উচ্চ গ্যাস ঘনত্বের জন্য অ্যানালগ মান (সম্পূর্ণ স্কেল)
// অ্যালার্ম ট্রিগার করার জন্য নতুন থ্রেশহোল্ড:
const int FIRE_ALARM_THRESHOLD = 5; // ৫ বা তার বেশি LED জ্বললে অ্যালার্ম ট্রিগার হবে এবং LCD-তে 'FIRE' দেখাবে।

void setup() {
  // সিরিয়াল যোগাযোগ শুরু করুন
  Serial.begin(9600);
  Serial.println("Gas Alarm System Initialized (গ্যাস অ্যালার্ম সিস্টেম শুরু হয়েছে)");

  // --- LCD Initialization (LCD শুরু করা) ---
  lcd.init();      // LCD শুরু করুন
  lcd.backlight(); // ব্যাকলাইট চালু করুন
  lcd.print("System Ready...");
  delay(1000);
  lcd.clear();

  // সমস্ত LED পিন আউটপুট হিসাবে সেট করুন
  for (int i = 0; i < NUM_LEDS; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW); // সব LED বন্ধ রাখুন
  }

  // বুজার পিন আউটপুট হিসাবে সেট করুন
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  // 1. সেন্সর থেকে মান পড়ুন
  int sensorValue = analogRead(GAS_SENSOR_PIN);

  // 2. সেন্সরের মানকে LED সংখ্যায় (0-8) ম্যাপ করুন
  int ledCount = map(sensorValue, GAS_MIN_READING, GAS_MAX_READING, 0, NUM_LEDS);
  ledCount = constrain(ledCount, 0, NUM_LEDS);

  // 3. LED ডিসপ্লে (বার গ্রাফ) আপডেট করুন
  for (int i = 0; i < NUM_LEDS; i++) {
    if (i < ledCount) {
      digitalWrite(LED_PINS[i], HIGH);
    } else {
      digitalWrite(LED_PINS[i], LOW);
    }
  }

  // 4. ফায়ার শর্ত এবং অ্যালার্ম/ডিসপ্লে পরীক্ষা করুন
  if (ledCount >= FIRE_ALARM_THRESHOLD) { // ৫ বা তার বেশি LED জ্বলেছে কিনা তা পরীক্ষা করুন
    // অ্যালার্ম শর্ত পূরণ হয়েছে: ৫ বা তার বেশি LED ON
    tone(BUZZER_PIN, 1500); // উচ্চ পিচে শব্দ বাজান

    // LCD-তে "FIRE is ON" বার্তা প্রদর্শন
    lcd.setCursor(0, 0); // কলাম 0, সারি 0 থেকে শুরু
    lcd.print("!!! FIRE is ON !!!");
    lcd.setCursor(0, 1); // কলাম 0, সারি 1
    lcd.print("LEDs ON: ");
    lcd.print(ledCount);
    lcd.print("/8 DANGER!");

    Serial.print("!!! FIRE ALARM !!! LED Count: ");
    Serial.println(ledCount);

  } else {
    // শর্ত পূরণ হয়নি: ৫ এর কম LED ON
    noTone(BUZZER_PIN); // বুজার বন্ধ করুন

    // LCD-তে "Status OK" বার্তা প্রদর্শন
    lcd.setCursor(0, 0);
    lcd.print("Status OK       "); // আগের লেখা মুছে ফেলার জন্য স্পেস
    lcd.setCursor(0, 1);
    lcd.print("LEDs ON: ");
    lcd.print(ledCount);
    lcd.print("/8 Safe.    "); // স্পেস দিয়ে লেখা পরিষ্কার করুন

    Serial.print("Status OK. LED Count: ");
    Serial.println(ledCount);
  }

  // লুপকে নিয়ন্ত্রণযোগ্য রাখতে ছোট বিরতি
  delay(100);
}