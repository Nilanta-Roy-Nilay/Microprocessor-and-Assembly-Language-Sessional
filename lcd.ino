#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define BUZZER_PIN 13
#define PIN_LED_1 6
#define PIN_LED_2 5
#define PIN_LED_3 4
#define PIN_LED_4 3
#define PIN_LED_5 2
#define PIN_INDICATOR 7
#define PIN_GAS A3
#define SERVO_PIN 9

Servo doorServo;
LiquidCrystal_I2C lcd(0x26, 16, 2); // LCD I2C address may be 0x27 or 0x3F

void setup() {
  pinMode(PIN_LED_1, OUTPUT);
  pinMode(PIN_LED_2, OUTPUT);
  pinMode(PIN_LED_3, OUTPUT);
  pinMode(PIN_LED_4, OUTPUT);
  pinMode(PIN_LED_5, OUTPUT);
  pinMode(PIN_INDICATOR, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  doorServo.attach(SERVO_PIN);
  doorServo.write(0); // Door closed initially

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Gas Monitor Ready");
  delay(2000);
  lcd.clear();

  Serial.begin(9600);
}

void loop() {
  int gasValue = analogRead(PIN_GAS);
  int value = map(gasValue, 300, 750, 0, 100);
  value = constrain(value, 0, 100);

  // Indicator LED blink
  digitalWrite(PIN_INDICATOR, millis() / 500 % 2);

  // LED bar display
  digitalWrite(PIN_LED_1, HIGH);
  digitalWrite(PIN_LED_2, value >= 20 ? HIGH : LOW);
  digitalWrite(PIN_LED_3, value >= 40 ? HIGH : LOW);
  digitalWrite(PIN_LED_4, value >= 60 ? HIGH : LOW);
  digitalWrite(PIN_LED_5, value >= 80 ? HIGH : LOW);

  // Buzzer
  if (value >= 50) {
    long frequency = map(value, 0, 100, 1500, 2500);
    tone(BUZZER_PIN, frequency, 200);
  } else {
    noTone(BUZZER_PIN);
  }

  // Servo door control
  if (value >= 80) {
    doorServo.write(90); // Open door
  } else {
    doorServo.write(0);  // Close door
  }

  // LCD Display
  lcd.clear();
  if (value >= 80) {
    lcd.setCursor(0, 0);
    lcd.print("GAS HIGH LEVEL!");
    lcd.setCursor(0, 1);
    lcd.print("PLEASE LEAVE!");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Gas Level: ");
    lcd.print(value);
    lcd.print("%  ");
    lcd.setCursor(0, 1);
    lcd.print("Door: ");
    lcd.print(value >= 80 ? "OPEN " : "CLOSED");
  }

  Serial.print("Gas Level: ");
  Serial.println(value);
  delay(500);
}

