#include <LiquidCrystal.h>
unsigned long lastButtonDebounce=0,remTime=0;
bool start=0;
int state=0,p1Score=0,p2Score=0;

LiquidCrystal lcd(2,3,4,5,6,7);

byte customChar1[] = {
  B11111,B11101,B11011,B11101,B11111,B10000,B10000,B10000
};
byte customChar2[] = {
  B11111,B11101,B11011,B11101,B11111,B00001,B00001,B00001
};

void welcome(){
  lcdPrint(5,0,"WELCOME!");
  delay(500);
  }

  //////////////////////                FUNCTION OVERLOADING                 //////////////////////     

  
    void lcdPrint(int col,int row, const String &prompt){                   //FOR PRINTING TEXT
      lcd.setCursor(col,row);       
    lcd.print(prompt);
    }
    void lcdPrint(int col,int row, int val){                                //FOR PRINTING INTEGER
    lcd.setCursor(col,row);
    lcd.print(val);
    }
  void lcdPrint(int col,int row,const String &prompt, int val){
    lcd.setCursor(col,row);                                                 //FOR PRINTING TEXT AND INTEGER
    lcd.print(prompt + val);
    }
    
//////////////////////                FUNCTION OVERLOADING                 //////////////////////     





    
void setup() {
  lcd.begin(16,2);
  pinMode(8,INPUT);
  welcome();
  randomSeed(analogRead(0));
  lcd.createChar(1, customChar1);
   lcd.createChar(2, customChar2);
  // put your setup code here, to run once:
 
}




void player1lcd(){
  lcd.clear();
 lcdPrint(3,0,"PLAYER 1: ");
  if(!p1Score){
    int randomDuration=random(50,100);
    for(int i=0;i<=randomDuration;i++){
      lcd.setCursor(8,1);
      p1Score=random(1,7);
      lcd.print(p1Score);
      delay(10);
    }
    
    }
lcdPrint(8,1,p1Score);

  }







  
void player2lcd(){
  lcd.clear();
  lcdPrint(3,0,"PLAYER 2: ");
  if(!p2Score){
    int randomDuration=random(50,100);
    for(int i=0;i<=randomDuration;i++){
      lcd.setCursor(8,1);
      p2Score=random(1,7);
      lcd.print(p2Score);
      delay(10);
    }
    }
 lcdPrint(8,1,p2Score);
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







    
 void flag(){
      lcd.setCursor(0,0);
      lcd.write(1);
      lcd.setCursor(15,0);
      lcd.write(2);
  }






  
void compareScores(){
  lcd.clear();
  if(p1Score>p2Score){
      lcdPrint(4,0,"WINNER");
      lcdPrint(3,1,"PLAYER 1: ",p1Score);
      flag();
    }
    else if(p1Score<p2Score){
      lcdPrint(4,0,"WINNER");
      lcdPrint(3,1,"PLAYER 2: ",p2Score);
      flag();
    }
    else {
      lcdPrint(5,0,"DRAW");
      lcdPrint(4,0,"P1: ",p1Score);
      lcdPrint(10,0,"P2: ",p2Score);
        flag();
    }
  }










void loop() {
  bool buttonTriggered=digitalRead(8);
  if(millis()-lastButtonDebounce>=50){
    lastButtonDebounce=millis();
    if(buttonTriggered){
      start=1;
      state++;}
    }
 if(start){
  lcd.noBlink();
  if(state==1){
      player1lcd();
      state++;
      }
      else if(state==2){
          idle(10,1);
        }
        else if(state==3){
          player2lcd();
          delay(1000);
        compareScores();
        state++;
          }
          else if(state==4){
          idle(10,0);
        }
          else{
            state=1;
            p1Score=0;
            p2Score=0;
            }
  }
  else{
    lcdPrint(2,0,"PRESS BUTTON");
    lcdPrint(4,1,"TO START");
    idle(13,1);
    }
    


  

}
