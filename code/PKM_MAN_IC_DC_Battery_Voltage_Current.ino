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

const int numReadings1 = 10;
int readings1[numReadings1];
int readIndex1 = 0;
int total1 = 0;

const int numReadings2 = 10;
int readings2[numReadings2];
int readIndex2 = 0;
int total2 = 0;

double batasarus = 2485;
double naikarus = 185;

int adctegangan1, adctegangan2, tegangan, tegangan2, Pow_Bat;
float vBat, cal_vol_bat, cal_cur_bat, arusdc, arusdc1, iBat, average;
double adcarus, adcarus1, varus, varus1;

//SYMBOLS
byte Bat[] = { 0b01110, 0b11011, 0b10001, 0b11011, 0b11111, 0b11111, 0b10001, 0b11111 };
byte DC[] = { 0b11000, 0b10100, 0b10100, 0b11000, 0b00011, 0b00100, 0b00100, 0b00011 };
byte Vol[] = { 0b00100, 0b01000, 0b11100, 0b01001, 0b10010, 0b00111, 0b00010, 0b00100 };
byte Cur[] = { 0b00100, 0b01010, 0b10101, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100 };
byte Pow[] = { 0b00100, 0b10101, 0b10101, 0b10101, 0b10101, 0b10001, 0b10001, 0b01110 };

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  Wire.begin();
  lcd.init();
  lcd.backlight();
  lcd.setBacklight(HIGH);
  lcd.createChar(1, DC);
  lcd.createChar(2, Bat);
  lcd.createChar(3, Vol);
  lcd.createChar(4, Cur);
  lcd.createChar(5, Pow);
}

void CalDCVolt() {
  //Battery's Voltage DC
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
  vBat = tegangan / 10.0;
  cal_vol_bat = (vBat + 0.1) / 1;
  if (cal_vol_bat <= 0.2) {
    cal_vol_bat = 0.0;
  }
  // Serial.print("Voltage vBat: ");
  // Serial.println(cal_vol_bat);
}


void CalDCCurBat(){
  //DCCurrentBat
  adcarus1 = analogRead(A1);
  total1 = total1 - readings1[readIndex1];
  readings1[readIndex1] = adcarus1;
  total1 = total1 + readings1[readIndex1];
  readIndex1 = readIndex1 + 1;
  if (readIndex1 >= numReadings1) {
    readIndex1 = 0;
  }
  adcarus1 = total1 / numReadings1;
  varus1 = (adcarus1 / 1024.0) * 5000;
  arusdc1 = ((varus1 - batasarus) / naikarus);
  cal_cur_bat = (arusdc1 + 0.6073) / 1.1482;
  if (cal_cur_bat <= 0.05) {
    cal_cur_bat = 0.0;
  }
  Serial.print("DC CURRENT: ");
  // Serial.println(cal_cur_bat);
  Serial.println(arusdc1);
  delay(500);
}

void CalPower() {
  //Power for Battery
  Pow_Bat = cal_vol_bat * cal_cur_bat;
}

void loop() {
  // put your main code here, to run repeatedly:
  CalDCVolt();
  CalDCCurBat();
  CalPower();
  if (updateData) {
    // Store values in EEPROM only if updateData is true
    EEPROM.write(address, static_cast<byte>(cal_vol_bat));
    EEPROM.write(address + 1, static_cast<byte>(cal_cur_bat));
    EEPROM.write(address + 2, static_cast<byte>(Pow_Bat));
  }
  buttonState = digitalRead(buttonPin);
  //Create table in LCD
  //Make the output more observable for customer
  lcd.setCursor(1, 0);
  lcd.print("VOL");
  lcd.write(3);  //Logo for Voltage
  lcd.setCursor(6, 0);
  lcd.print("CUR");
  lcd.write(4);  //Logo for Current
  lcd.setCursor(12, 0);
  lcd.print("POW");
  lcd.write(5);  //Logo for Power
  lcd.setCursor(0, 0);
  lcd.write(1);  //Logo for DC
  lcd.setCursor(0, 1);
  lcd.write(2);  //Logo for Battery
  //End of Table Design
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
  } else {
    //Start giving output data here:
    //Measure Battery's Voltage...
    lcd.setCursor(1, 1);
    lcd.print(cal_vol_bat);
    lcd.setCursor(5, 1);
    lcd.print("V");
    //End of first column

    //Measure Battery's Current
    lcd.setCursor(6, 1);
    lcd.print(cal_cur_bat, 2);
    lcd.setCursor(10, 1);
    lcd.print("A");
    //End of second column

    //Calculation of Power using "P = V * I" formula
    //Battery's Power:
    lcd.setCursor(12, 1);
    lcd.print(Pow_Bat);
    lcd.setCursor(15, 1);
    lcd.print("W");
    // Resume updating data
    updateData = true;
  }
  delay(1000);  // Add a small delay for stability, if needed
}
