#include <LiquidCrystal.h>
LiquidCrystal lcd(7,6,5,4,3,2);
int mins=0,sec=0,state=0,mils=0;
unsigned long lastSecond=0,remTime=0;
char L0[17],L1[17];
extern volatile unsigned long timer0_millis;
void setup() {
  lcd.begin(16,2);
  for(int i=8;i<=10;i++)pinMode(i,INPUT);
}

void lcdPrintTime(int col,int row){
    lcd.setCursor(col,row);
    if(row){
      sprintf(L1,"%-2.2d:%-2.2d:%-5.2d",mins,sec,mils);
      lcd.print(L1);
      }
    else{
      sprintf(L0,"%-2.2d:%-2.2d:%-5.2d",mins,sec,mils);
      lcd.print(L0);
      }
  }
void lcdPrint(int col,int row, char text[17]){
    lcd.setCursor(col,row);
    if(row){
      sprintf(L1,"%-16s",text);
      lcd.print(L1);
      }
    else{
      sprintf(L0,"%-16s",text);
      lcd.print(L0);
      }
  }

  
void idle(int col, int row){
    if(millis()-remTime>450)remTime=millis();
    else if(millis()-remTime>=350){
        lcdPrint(col,row,"...");
      }
    else if(millis()-remTime>=250){
        lcdPrint(col,row,".. ");
      }
    else if(millis()-remTime>=150){
       lcdPrint(col,row,".  ");
      }
    else if(millis()-remTime>=50){
        lcdPrint(col,row,"   ");
      }
    
    }
void loop() {
  
  if(digitalRead(8)){
    state=1;
    }
   else if(digitalRead(9)){
    state=2;
    }
   else if(digitalRead(10)){
    state=3;
    
    }
  if(state==1){
    lcdPrint(0,0,"STOPWATCH");
    idle(9,0);
    mils=millis() - lastSecond;
    if(mils>=1000){
      lastSecond=millis();
      sec++;
      if(sec==60){
        mins++;
        sec=0;
        }
      }
    }
    else if(state==2){
      noInterrupts ();
      timer0_millis = mils;
      interrupts ();
      lastSecond=0;
      lcdPrint(0,0,"PAUSED");
      idle(6,0);
      }
    else if(state==3){
        lcdPrint(0,0,"RESETTING");
      _delay_ms(1000);
      state=0;
      }
    else{
        lcdPrint(0,0,"PRESS START");
        idle(11,0);
        mins=0;sec=0,mils=0,lastSecond=0;
        noInterrupts ();
      timer0_millis = 0;
      interrupts ();
      }
     lcdPrintTime(0,1);

}
