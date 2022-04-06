#include <LiquidCrystal.h>
LiquidCrystal lcd(7,6,5,4,3,2);
int phLevel=0;

void setup() {
  lcd.begin(16,2);
  pinMode(13,OUTPUT);
  pinMode(A1,INPUT);
}
  
void loop() {
 phLevel=analogRead(A1);
 int val=map(phLevel,0,1024,0,11);
 lcd.setCursor(0,0);
 lcd.print(val);
 _delay_ms(50);

}
