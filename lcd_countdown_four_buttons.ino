#include <LiquidCrystal.h>
LiquidCrystal lcd(7,6,5,4,3,2);
int state=0;
long counter=90;
unsigned long lastButtonDebounce=0,lastCount=0,remTime=0,lastAdd=0;
char L0[17],L1[17];
void setup() {
  lcd.begin(16,2);
  for(int  i=8;i<=12;i++)pinMode(i,INPUT);
  // put your setup code here, to run once:

}
void lcdPrint(int col,int row, char text[17], int val){
    lcd.setCursor(col,row);
    if(row){
      sprintf(L1,"%s %-4d",text,val);
      lcd.print(L1);
      }
    else{
      sprintf(L0,"%s %-4d",text,val);
      lcd.print(L0);
      }
  }
  
void lcdPrint(int col,int row, char text[17]){
    lcd.setCursor(col,row);
    if(row){
      sprintf(L1,"%s",text);
      lcd.print(L1);
      }
    else{
      sprintf(L0,"%s",text);
      lcd.print(L0);
      }
  }
void lcdPrint(int col,int row, int val){
  lcd.setCursor(col,row);
  lcd.print(val);
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
  bool resetTriggered =digitalRead(12);
  bool startTriggered =digitalRead(11);
  bool stopTriggered =digitalRead(10);
  bool addTriggered = digitalRead(9);
  bool minTriggered = digitalRead(8);
  if(millis()-lastButtonDebounce>=50){
      lastButtonDebounce=millis();
      if(startTriggered)state=1;
      else if(stopTriggered)state=2;
      else if(addTriggered && state!=1)state=3;
      else if (minTriggered&& state!=1)state=4;
      else if(resetTriggered)state=5;
    }
switch(state){
  case 1: 
          if(millis()-lastCount>=500){
            lastCount=millis();
            counter++;
            }
            break;
  case 2:
          idle(11,0);
          break;

  case 3:
        counter+=1;
        _delay_ms(200);
        state=2;
        break;
  case 4:
        counter--;
        _delay_ms(200);
        state=2;
        break;
  case 5:
        counter=60;
        state=2;
        break;
  default :
        idle(11,0);
  }
lcdPrint(0,0,"COUNT DOWN");
lcdPrint(0,1,"Seconds: ",counter);


    
}
























  
