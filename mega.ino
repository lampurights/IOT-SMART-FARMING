#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

SoftwareSerial espSerial(5, 6);
LiquidCrystal_I2C lcd(0x27, 16, 2);
 
void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.print("IRRIGATION");
  lcd.setCursor(0, 1);
  lcd.print("SYSTEM IS ON ");
    lcd.print("");
    delay(3000);
  lcd.clear();
}
 
void loop() {
  int water_level = analogRead(A8);
  int value = analogRead(A0);
  String dataToSend = String(water_level) + "," + String(value) ;
  // static unsigned long previousMillis = 0;
  // unsigned long currentMillis = millis();
  // if (currentMillis - previousMillis >= 3000) {//decrease to test and debyg every 3 secound
        // espSerial.println("Sending data to NodeMCU...");
  espSerial.println(dataToSend);
  Serial.println(dataToSend);
      // previousMillis = currentMillis;
    // }
  if (water_level > 500){
    lcd.clear();
    if (value > 950) {
      digitalWrite(2, LOW);
      lcd.setCursor(0, 0);
      lcd.print("Water Pump is ON ");
    } else {
      digitalWrite(2, HIGH);
      lcd.setCursor(0, 0);
      lcd.print("Water Pump is OFF");
    }
    if (value < 300) { 
      lcd.setCursor(0, 1);
      lcd.print("Moisture : HIGH");
    } else if (value > 300 && value < 950) {
      lcd.setCursor(0, 1);
      lcd.print("Moisture : MID ");
    } else if (value > 950) {
      lcd.setCursor(0, 1);
      lcd.print("Moisture : LOW ");
    }
  }
  else
    {
      lcd.setCursor(0, 0);
      lcd.print("water level: low");
      digitalWrite(2, HIGH);
    }
  delay(3000);
}