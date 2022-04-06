#include <LiquidCrystal.h>
LiquidCrystal lcd(7,6,5,4,3,2);
int p1min=1,p2min=1,p1sec=30,p2sec=30,state=0,prevState=0;
char L0[17],L1[17];
extern volatile unsigned long timer0_millis;
unsigned long lastSecond=0,remTime=0;
void setup() {
 lcd.begin(16,2);
 for(int i=8;i<=9;i++)pinMode(i,INPUT);
}
void lcdPrint(int col,int row,char text[17], int mins, int sec){
    lcd.setCursor(col,row);
    if(row){
      sprintf(L1,"%s %-2.2d %-2.2d",text,mins,sec);
      lcd.print(L1);
      }
    else{
      sprintf(L0,"%s %-2.2d %-2.2d",text,mins,sec);
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

void countDown(int &mins,int &sec){
    if(millis()-lastSecond>=100){
        lastSecond=millis();
        if(sec)sec--;
        if(!sec && mins){
            mins--;
            sec=59;
          }
      }
  
  }
void announceWinner(){
    if(!p1sec&&!p1min){
            lcdPrint(0,0,"CONGRATS");
            lcdPrint(0,1,"PLAYER 2");
          }
    else if(!p2min&&!p2sec){
             lcdPrint(0,0,"CONGRATS");
            lcdPrint(0,1,"PLAYER 1");
          }
  }
void loop() {
  if(digitalRead(8)){
    state=1;
   
    }
  if(digitalRead(9)&& prevState==1){
    state=2;
   
  }
  if((!p1sec&&!p1min) || (!p2min&&!p2sec)){
     state=3;   
      }


    
  if(state==1){
     prevState=1;
    countDown(p2min,p2sec);
    }
   else if(state==2 ){
    countDown(p1min,p1sec);
    }
   else if(state==3){
     announceWinner();
     _delay_ms(2000);
     p1min=0,p2min=0,p1sec=30,p2sec=30;
     state=0;
     prevState=0;
    } 





   
  if(state==1 || state==2){
      lcdPrint(0,0,"PLAYER1: ",p1min,p1sec);
      lcdPrint(0,1,"PLAYER2: ",p2min,p2sec);
    }
  else if(!state ) {
    lcdPrint(0,0,"PRESS START");
    lcdPrint(0,1,"PLAYER 1");
    idle(8,1);
    }
    
}
