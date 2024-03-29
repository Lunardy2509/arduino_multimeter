#include <Wire.h>               // Library for Wire
#include <LiquidCrystal_I2C.h>  // Library for LCD using I2C communication
#include <EEPROM.h>             // Library for store and read data in memory

LiquidCrystal_I2C lcd(0x27, 16, 2);
String Data;
char c;

//for EEPROM Store Data
int buttonPin = 4;
int buttonState = 0;
int ledPin = 3;
int address = 0;
bool updateData = true;
//

const int numReadings = 10;
int readings[numReadings];
int readIndex = 0;
int total = 0;

const int numReadings1 = 10;
int readings1[numReadings1];
int readIndex1 = 0;
int total1 = 0;

const int numReadings2 = 10;
int readings2[numReadings2];
int readIndex2 = 0;
int total2 = 0;

const int numReadings3 = 10;
int readings3[numReadings3];
int readIndex3 = 0;
int total3 = 0;

double batasarus = 2485;
double naikarus = 185;

int adctegangan1, adctegangan2, tegangan, tegangan2, Pow_PV;
float cal_vol_pv, cal_cur_pv, arusdc, arusdc1, iPV, average, vPV;
double adcarus, adcarus1, varus, varus1;
char incomingChar;

// SYMBOLS
byte PV[] = { 0b11011, 0b10101, 0b11011, 0b10101, 0b11011, 0b10101, 0b11011, 0b10101 };
byte DC[] = { 0b11000, 0b10100, 0b10100, 0b11000, 0b00011, 0b00100, 0b00100, 0b00011 };
byte Vol[] = { 0b00100, 0b01000, 0b11100, 0b01001, 0b10010, 0b00111, 0b00010, 0b00100 };
byte Cur[] = { 0b00100, 0b01010, 0b10101, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100 };
byte Pow[] = { 0b00100, 0b10101, 0b10101, 0b10101, 0b10101, 0b10001, 0b10001, 0b01110 };

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
  // pinMode(ledPin, OUTPUT);
  Wire.begin();
  lcd.init();
  lcd.backlight();
  lcd.setBacklight(HIGH);
  lcd.createChar(1, DC);
  lcd.createChar(2, PV);
  lcd.createChar(3, Vol);
  lcd.createChar(4, Cur);
  lcd.createChar(5, Pow);
}

void CalDCVolt() {
  //PV's Voltage DC
  adctegangan1 = analogRead(A0);
  total2 = total2 - readings2[readIndex2];
  readings2[readIndex2] = adctegangan1;
  total2 = total2 + readings2[readIndex2];
  readIndex2 = readIndex2 + 1;
  if (readIndex2 >= numReadings2) {
    readIndex2 = 0;
  }
  adctegangan1 = total2 / numReadings2;
  tegangan = map(adctegangan1, 0, 1024, 0, 250);
  vPV = tegangan / 10.0;
  cal_vol_pv = (vPV + 0.0545) / 1.2238;
  if (cal_vol_pv <= 0.2) {
    cal_vol_pv = 0.0;
  }
}

void CalDCCurPV() {
  //DCCurrentPV
  adcarus = analogRead(A1);
  total = total - readings[readIndex];
  readings[readIndex] = adcarus;
  total = total + readings[readIndex];
  readIndex = readIndex + 1;
  if (readIndex >= numReadings) {
    readIndex = 0;
  }
  adcarus = total / numReadings;
  varus = (adcarus / 1024.0) * 5000;
  arusdc = ((varus - batasarus) / naikarus);
  cal_cur_pv = (arusdc - 3.7172) / -1.129;
  if (cal_cur_pv <= 0.05){
    cal_cur_pv = 0.0;
  }
  delay(500);
}

void CalPower() {
  //Power for PV
  Pow_PV = cal_vol_pv * cal_cur_pv;
}

void loop() {
  // put your main code here, to run repeatedly:
  CalDCVolt();
  CalDCCurPV();
  CalPower();
  if (updateData) {
    // Store values in EEPROM only if updateData is true
    EEPROM.write(address, static_cast<byte>(cal_vol_pv));
    EEPROM.write(address + 1, static_cast<byte>(cal_cur_pv));
    EEPROM.write(address + 2, static_cast<byte>(Pow_PV));
  }
  // reading the button
  buttonState = digitalRead(buttonPin);
  lcd.setCursor(1, 0);
  lcd.print("VOL");
  lcd.write(3);
  lcd.setCursor(6, 0);
  lcd.print("CUR");
  lcd.write(4);
  lcd.setCursor(12, 0);
  lcd.print("POW");
  lcd.write(5);
  lcd.setCursor(0, 0);
  lcd.write(1);
  lcd.setCursor(0, 1);
  lcd.write(2);

  if (buttonState == LOW) {
    // Read values from EEPROM and send them over Serial
    updateData = false;  // Stop updating data
    // digitalWrite(ledPin, HIGH);
    lcd.setCursor(1, 1);
    lcd.print(EEPROM.read(address));
    lcd.setCursor(5, 1);
    lcd.print("V");
    lcd.setCursor(6, 1);
    lcd.print(EEPROM.read(address + 1));
    lcd.setCursor(10, 1);
    lcd.print("A");
    lcd.setCursor(12, 1);
    lcd.print(EEPROM.read(address + 2));
    lcd.setCursor(15, 1);
    lcd.print("W");
  }

  else {
    // Print current sensor values
    // digitalWrite(ledPin, LOW);
    lcd.setCursor(1, 1);
    lcd.print(cal_vol_pv);
    lcd.setCursor(5, 1);
    lcd.print("V");

    lcd.setCursor(6, 1);
    lcd.print(cal_cur_pv, 2);
    lcd.setCursor(10, 1);
    lcd.print("A");

    lcd.setCursor(12, 1);
    lcd.print(Pow_PV);
    lcd.setCursor(15, 1);
    lcd.print("W");
    // Resume updating data
    updateData = true;
  }
  delay(1000);  // Add a small delay for stability, if needed
}