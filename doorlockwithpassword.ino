#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

Servo myservo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define Password_Length 7   // 6 digit + NULL

char Data[Password_Length];
char Master[Password_Length] = "123456";

byte data_count = 0;
int pos = 0;
bool door = true;
char customKey;

/* Keypad setup */
const byte ROWS = 4;
const byte COLS = 3;

char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[ROWS] = {1,2,3,4};
byte colPins[COLS] = {5,6,7};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

/* ================= SETUP ================= */
void setup() {
  myservo.attach(9);
  ServoClose();

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print(" Smart Door ");
  lcd.setCursor(0,1);
  lcd.print(" System Ready ");
  delay(2000);
  lcd.clear();
}

/* ================= LOOP ================= */
void loop() {

  if (customKey == '#') {
    lcd.clear();
    clearData();
  }

  if (door == false) {
    customKey = keypad.getKey();
    if (customKey == '*') {
      lcd.clear();
      ServoClose();
      lcd.setCursor(0,0);
      lcd.print(" Door Locked ");
      delay(2000);
      door = true;
    }
  }
  else {
    Open();
  }
}

/* ================ FUNCTIONS ================ */

void clearData() {
  while (data_count != 0) {
    Data[--data_count] = 0;
  }
}

void ServoOpen() {
  for (pos = 180; pos >= 0; pos -= 5) {
    myservo.write(pos);
    delay(15);
  }
}

void ServoClose() {
  for (pos = 0; pos <= 180; pos += 5) {
    myservo.write(pos);
    delay(15);
  }
}

void Open() {
  lcd.setCursor(0,0);
  lcd.print("Enter Password");

  customKey = keypad.getKey();
  if (customKey) {
    Data[data_count] = customKey;
    lcd.setCursor(data_count,1);
    lcd.print(Data[data_count]);   // show real password
    data_count++;
  }

  if (data_count == Password_Length - 1) {
    if (!strcmp(Data, Master)) {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Access Granted");
      ServoOpen();
      door = false;
    } else {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Access Denied");
      delay(2000);
    }
    clearData();
    lcd.clear();
  }
}