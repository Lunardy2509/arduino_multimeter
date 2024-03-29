#include <Wire.h>               // Library for Wire
#include <LiquidCrystal_I2C.h>  // Library for LCD using I2C communication
#include <ACS712.h>             // Library for ACS712 Current Sensor
#include <ZMPT101B.h>           // Library for ZMPT101B AC Voltage Sensor
#include <EEPROM.h>             // Library for store and read data in memory
#define SENSITIVITY 500.0f
LiquidCrystal_I2C lcd(0x27, 16, 2);
int vInv, cal_VInv, Pow_Inv;
float iInv, cal_iInv, average;

//for EEPROM Store Data
int buttonPin = 4;
int buttonState = 0;
int ledPin = 3;
int address = 0;
bool updateData = true;
//

ACS712 ACS(A1, 5.0, 1023, 185);
//  Arduino UNO has 5.0 volt with a max ADC value of 1023 steps
//  ACS712 5A  uses 185 mV per A
//  ACS712 20A uses 100 mV per A
//  ACS712 30A uses  66 mV per A
uint32_t start, stop;

// ZMPT101B sensor output connected to analog pin A6
// and the voltage source frequency is 50 Hz.
ZMPT101B voltageSensor(A0, 50.0);

//SYMBOLS
byte Inv[] = { 0b00100, 0b01010, 0b00100, 0b00000, 0b00100, 0b01010, 0b00100, 0b00000 };
byte AC[] = { 0b01000, 0b10100, 0b11100, 0b10100, 0b00011, 0b00100, 0b00100, 0b00011 };
byte Vol[] = { 0b00100, 0b01000, 0b11100, 0b01001, 0b10010, 0b00111, 0b00010, 0b00100 };
byte Cur[] = { 0b00100, 0b01010, 0b10101, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100 };
byte Pow[] = { 0b00100, 0b10101, 0b10101, 0b10101, 0b10101, 0b10001, 0b10001, 0b01110 };

void
setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  Wire.begin();
  voltageSensor.setSensitivity(SENSITIVITY);
  lcd.init();
  lcd.backlight();
  lcd.setBacklight(HIGH);
  lcd.createChar(1, AC);
  lcd.createChar(2, Inv);
  lcd.createChar(3, Vol);
  lcd.createChar(4, Cur);
  lcd.createChar(5, Pow);
}

void CalACVolt() {
  vInv = voltageSensor.getRmsVoltage();
  cal_VInv = (vInv - 0.7436) / 0.9698;
  if (cal_VInv < 20) {
    cal_VInv = 0;
  }
  // Serial.print("Vinv: ");
  // Serial.println(vInv);
  // Serial.print("calibrated: ");
  // Serial.println(cal_VInv);
  delay(500);
}

void CalACCurrent() {
  average = 0;
  for (int i = 0; i < 100; i++) {
    average += ACS.mA_AC();
  }
  iInv = average / 100.0;
  iInv = iInv / 1000;
  cal_iInv = (iInv - 0.2373) / 0.4419;
  if (cal_iInv <= 0.04) {
    cal_iInv = 0.0;
  }
  // Serial.println(iInv);
  // Serial.println(cal_iInv);
  delay(500);
}

void CalPower() {
  //Power for Inverter
  Pow_Inv = cal_VInv * cal_iInv * 0.54;
}

void loop() {
  CalACVolt();
  CalACCurrent();
  CalPower();
  if (updateData) {
    // Store values in EEPROM only if updateData is true
    EEPROM.write(address, static_cast<byte>(cal_VInv));
    EEPROM.write(address + 1, static_cast<byte>(cal_iInv));
    EEPROM.write(address + 2, static_cast<byte>(Pow_Inv));
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
  lcd.print("S");
  lcd.write(5);  //Logo for Power
  lcd.setCursor(0, 0);
  lcd.write(1);  //Logo for Inverter
  lcd.setCursor(0, 1);
  lcd.write(2);  //Logo for AC
  //End of Table Design
  if (buttonState == LOW) {
    // Read values from EEPROM and send them over Serial
    updateData = false;  // Stop updating data
    // digitalWrite(ledPin, HIGH);
    lcd.setCursor(1, 1);
    lcd.print(EEPROM.read(address));
    lcd.setCursor(4, 1);
    lcd.print("V");
    lcd.setCursor(6, 1);
    lcd.print(EEPROM.read(address + 1));
    lcd.setCursor(10, 1);
    lcd.print("A");
    lcd.setCursor(12, 1);
    lcd.print(EEPROM.read(address + 2));
    lcd.setCursor(14, 1);
    lcd.print("VA");
  } else {
    //Start giving output data here:
    //Measure Inverter's Voltage...
    lcd.setCursor(1, 1);
    lcd.print(cal_VInv);
    lcd.setCursor(4, 1);
    lcd.print("V");
    //End of first column

    //Measure Inverter's Current
    lcd.setCursor(6, 1);
    lcd.print(cal_iInv);
    lcd.setCursor(10, 1);
    lcd.print("A");
    //End of second column

    //Calculation of Power using "P = V * I" formula
    //Inverter's Power:
    lcd.setCursor(12, 1);
    lcd.print(Pow_Inv);
    lcd.setCursor(14, 1);
    lcd.print("VA");
    // Resume updating data
    updateData = true;
  }
  delay(1000);  // Add a small delay for stability, if needed
}