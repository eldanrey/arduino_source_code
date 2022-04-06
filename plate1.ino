//  #include <LiquidCrystal.h>
//  LiquidCrystal lcd(7,6,5,4,3,2);


  #include <LiquidCrystal_I2C.h>      //LIBRARY TO USE LCD FUNCTIONS
  LiquidCrystal_I2C lcd(0x27, 16, 2);     //INITIALIZE THE LCD_I2C FOUND ADDRESS @ 0x27 WITH SCANNER WITH 16 Columns and 2 Rows
  void setup() {
//    lcd.begin(16,2);
    lcd.begin();
    lcd.backlight();
    for(int i=2;i<=7;i++)pinMode(i,INPUT_PULLUP);
    pinMode(A3,INPUT);
    Serial.begin(9600);
  }



char L0[17],L1[17];
byte state=1;
int currFloor=-1,selectedFloor=-1;
String floors[]={"[G]","[2]","[3]","[4]"};
int upDownChoice;

void lcdPrint(int col,int row, String stext ){
  char text[17];
  stext.toCharArray(text,17);
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
void lcdPrint(int col,int row, String stext, String stext1 ){
  char text[17];
  char text1[17];
  stext1.toCharArray(text1,17);
  stext.toCharArray(text,17);
    lcd.setCursor(col,row);
    if(row){
      sprintf(L1,"%s %s",text,text1);
      lcd.print(L1);
      }
    else{
      sprintf(L0,"%s %s",text,text1);
      lcd.print(L0);
      }
  }  
void lcdPrint(int col,int row, int val){
    lcd.setCursor(col,row);
    if(row){
      sprintf(L1," %-4d",val);
      lcd.print(L1);
      }
    else{
      sprintf(L0,"%-4d",val);
      lcd.print(L0);
      }
  }
  
void lcdClear(){
  lcdPrint(0,0," ");
  lcdPrint(0,1," ");
  }
void elevatorDoor(){
    lcdPrint(2,1,"DOOR OPENED");
    _delay_ms(1000);
    lcdPrint(2,1,"DOOR CLOSED");
    _delay_ms(500);
  }
void printFloors(){
  lcdPrint(0,1,"   G  2  3  4");
  }
void currentFloor(){
  lcdPrint(0,0,"    ELEVATOR");
  printFloors();
  }
void selectFloor(){
  lcdPrint(0,0,"     SELECT");
  printFloors();
  }
void selectUPDOWN(){
  lcdPrint(0,0,"     SELECT");
  lcdPrint(0,1," [1]UP [2]DOWN");
  }
byte buttonPressed(){
  for(int i=2;i<=7;i++){
    if(!digitalRead(i))return (i-1);
    }

  }
void transferFloors(int cFloor, int sFloor, int udChoice){
  lcdClear();
    if(udChoice==1){
      for(int i=cFloor-1;i<=sFloor;i++){
        _delay_ms(500);
        lcdPrint(0,1,"     UP",floors[i]);
        _delay_ms(500);
        }
      }
     else if(udChoice==0){
        for(int i=cFloor-1;i>=sFloor-1;i--){
          _delay_ms(500);
          lcdPrint(0,1,"    DOWN",floors[i]);
          _delay_ms(500);
         
          }
      }
  }  


void loop() {

  

switch(state){
  case 1:
  currentFloor();
  if(buttonPressed()&&buttonPressed()<=4){
    Serial.print(buttonPressed());
    currFloor=buttonPressed();
    _delay_ms(200);
    state=2;
    lcdClear();
  elevatorDoor();
    }
  break;
  case 2:
  selectUPDOWN();
  if(buttonPressed()&&buttonPressed()>4){
    Serial.print(buttonPressed());
    upDownChoice=buttonPressed()-5;
    _delay_ms(200);
    state=3;
    }
  break;
  case 3:
  selectFloor();
  if(buttonPressed()&&buttonPressed()<=4){
    Serial.print(buttonPressed());
    selectedFloor=buttonPressed();
    _delay_ms(200);
    state=4;
    
    }
  break;
  case 4:
   Serial.print(buttonPressed());
  transferFloors(currFloor,selectedFloor,upDownChoice);
  _delay_ms(500);
  lcdClear();
  elevatorDoor();
  state=1;
  
  }
   
  
  


}
