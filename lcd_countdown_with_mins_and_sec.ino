#include <LiquidCrystal.h>
#include <DHT.h>
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(7,6,5,4,3,2);
const byte analogPin[3]={A0,A1,A2};
int mins=1, sec=0,state=0;
char L0[17],L1[17];
unsigned long lastCountDown=0,remTime=0;
void setup() {
  lcd.begin(16,2);
  for(int i=8;i<=11;i++)pinMode(i,INPUT);
  for(int i=0;i<3;i++)pinMode(analogPin[i],INPUT);
}
void countDown(){
  lcdPrint(0,0,"COUNTDOWN");
  if(millis()-lastCountDown>=500){
    lastCountDown=millis();
    if(!sec){
        if(mins>=1){
           sec=60;
           sec--;
           mins--;
          }
      }
     else sec--;
    }
    
  }
  
void lcdPrintTime(int row){
   lcd.setCursor(0,row);
   if(row){
      sprintf(L1,"Min: %-2d Sec: %-2d",mins,sec);
      lcd.print(L1);
    }
    else {
      sprintf(L0,"Min: %-2d Sec: %-2d",mins,sec);
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
  if(digitalRead(A0)){
    state=1;
    }
  if(digitalRead(A1)){
    state=2;
    }
   if(digitalRead(A2)){
      state=3;
    }
    if(state==1)countDown();
    else if(state==2 || !state){
      (state==2)?lcdPrint(0,0,"PAUSED"):lcdPrint(0,0,"PRESS START");
      (state==2)?idle(6,0):idle(11,0);
      if(digitalRead(8)){
        mins++;
        _delay_ms(200);
        }
        if(digitalRead(9)){
         if(sec==59){
          mins++;
          sec=0;
          }
        else sec++;
        _delay_ms(200);
        }
        if(digitalRead(10)){
        if(sec==0){
          mins--;
          sec=59;
          }
        else sec--;
        _delay_ms(200);
        }
        if(digitalRead(11)){
        mins--;
        _delay_ms(200);
        }
      }
     else if(state==3){
        mins=1;
        sec=0;
        lcdPrint(0,0,"RESET");
        _delay_ms(1000);
        state=0;
      }
 
   lcdPrintTime(1);
}
