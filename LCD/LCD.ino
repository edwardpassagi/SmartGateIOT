#include <LiquidCrystal.h>
const int RS = D3, EN = D4, d4 = D0, d5 = D8, d6 = D9, d7 = D10;
// D11 is SD2, D12 is SD3

LiquidCrystal lcd(RS, EN, d4, d5, d6, d7);
//LiquidCrystal lcd(D3, D4, D0, D8, D9, D10);

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  
  Serial.println("Hello, World!");
  lcd.print("Hello, World!");
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
//  Serial.println(millis() / 1000);
  lcd.print(millis() / 1000);
}
