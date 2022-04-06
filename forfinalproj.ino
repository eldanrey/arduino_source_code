
/*
FOR THE COMPLETION OF THE REQUIREMENTS FOR THE SUBJECT CPE330L
FINAL PROJECT PROPOSED BY:
1.) Eldan Rey Dofredo       - 3rd Year BSEE/CEE Student
2.) Edcir Jay Benitez       - 3rd Year BSEE/CEE Student
3.) Lloyd Greg Jaromahum    - 3rd Year BSEE/CEE Student



TODO to finish the software:

ADD THE EEPROM FUNCTION FOR SENSOR CALIBRATION
  EEPROM READ ON SETUP FUNCTION
  EEPROM WRITE FUNCTION

ADD THE LCD MENU
  SOIL MOISTURE SCREEN
  TEMPERATURE SCREEN
  HUMIDITY SCREEN
  VPD SCREEN
  

*/



//===============================LIBRARY INCLUDED FOR THE PROJECT========================================//

#include <Wire.h>                   //LIBRARY USED TO COMMUNICATE WITH THE I2C IN THE LCD
#include <LiquidCrystal_I2C.h>      //LIBRARY TO USE LCD FUNCTIONS
#include <EEPROM.h>
#include "DHT.h"                    //LIBRARY FOR DHT11

//=======================================================================================================//



//===========================DEFINING INPUT AND OUTPUT PINS==============================================//


#define DHTTYPE DHT11         
#define SoilMoistureSensor A0           //DEFINING THAT THE SOIL MOISTURE SENSOR IS CONNECTED TO A0 pin
#define Relay1 9                        //DEFINE THE 1st RELAY INPUT TO pin 9
#define Relay2 10                       //DEFINE THE 2nd RELAY INPUT TO pin 10
#define RedLed1 11                       //DEFINE THE RED LED TO PIN 11
#define RedLed2 12                       //DEFINE THE RED LED TO PIN 12
#define DHTPIN 2                        //DHT DATA PIN IS ON PIN 2 ON ARDUINO
#define EnterButton 3                   //DEFINE ENTER BUTTON TO PIN 3
#define EditButton 4                    //DEFINE EDIT BUTTON TO PIN 4
#define UpButton 5                      //DEFINE UP BUTTON TO PIN 5
#define DownButton 6                    //DEFINE DOWN BUTTON TO PIN 6
#define BottomFloatSwitch 7                  //DEFINE THE BOTTOM FLOAT SWITCH TO PIN 7    
#define HighFloatSwtich 8                  //DEFINE THE UPPER FLOAT SWITCH TO PIN 8
#define moistureLowerLimitAddress 1     //The moisture lower limit address in the EEPROM will be set to 1 
#define moistureUpperLimitAddress 3     //The moisture upper limit address in the EEPROM will be set to 3 (INTEGER HAS A SIZE OF 2 BYTES therefore we need 2 address for int data type)
#define vpdLowerLimitAddress 5           //THE lower limit for vpd address in EEPROM is set to 5 (FLOAT has a size of 4 bytes therefore we need 4 address slots to store them)
#define vpdUpperLimitAddress 9          //THE upper limit for vpd address in EEPROM is set to 9 



                                        //EVERY PIN IS DEFINED FOR READABILITY

//=======================================================================================================// 




//================================VARIABLE AND SENSORS INITIALIZATION====================================//


float lowerLimitVPD,upperLimitVPD,temp=0,hum=0;
int moistureUpperLimit,moistureLowerLimit,menuState=0;
char L0[17],L1[17];
LiquidCrystal_I2C lcd(0x27, 16, 2);     //INITIALIZE THE LCD_I2C FOUND ADDRESS @ 0x27 WITH SCANNER WITH 16 Columns and 2 Rows
DHT dht(DHTPIN, DHTTYPE);               //INITIALIZE DHT11 Sensor
extern volatile unsigned long timer0_millis;

//=======================================================================================================//




//=============================READS TEMPERATURE AND HUMIDITY TO GET VPD==================================//
float getVPD(){
  hum=dht.readHumidity();                                               //READS THE HUMIDITY USING DHT 11
  temp=dht.readTemperature();                                           //READS THE TEMPERATURE USING DHT11
  float vpsat = 0.6108*exp(((17.27*temp)/(temp+237.3)));                //CALCULATE THE VAPOR PRESSURE (VP) OF THE AIR AT CERTAI TEMPERATURE AND AT SATURATED HUMIDIY
  return(vpsat*((100-hum)/100));                                        //RETURNS THE VPD WHICH IS THE DIFFERENCE OF VP_SATURATED TO VP_ACTUAL THE UNIT IS kilopascal (kPA)
  }
//=======================================================================================================//




//========================GETS THE SOIL MOISTURE FROM SENSOR=============================================//
int getSoilMoisture(){
  int sumSoilMoisture=0;
  for(int i=0;i<16;i++){
    sumSoilMoisture+=analogRead(SoilMoistureSensor);            //AS THE SOIL MOISTURE SENSOR HAS A LOT OF NOISE IN ITS DATA
    _delay_ms(10);                                              //GETTING THE AVERAGE OF 16 READINGS WILL GIVE US LESS NOISE
    }                                                           //THIS WILL GIVE US MORE STABLE DATA FROM THE SENSOR
  return(sumSoilMoisture/16);                                 
  }
//=======================================================================================================//




//============================ARDUINO SETUP FUNCTION=====================================================//
void setup() {
  lcd.begin();                                  //THIS WILL INITIALIZE THE LCD AND START IT
  dht.begin();                                  //INITIALIZE THE DHT
  lcd.backlight();                              //TURNING ON THE BACKLIGHT OF THE LCD
  Serial.begin(9600);
  pinMode(SoilMoistureSensor,INPUT);            //INITIALIZE THE PINMODE OF THE SOIL MOISTURE (A0 pin) and set to INPUT
  for(int i=2;i<=6;i++)pinMode(i,INPUT);         //INITIALIZE THE INPUT PINS (DHT,4 Buttons, 2Float Switch)
  for(int i=9;i<=12;i++)pinMode(i,OUTPUT);      //INITIALIZE THE OUTPUT PINS (Relays, LEDs)
  for(int i=3;i<=8;i++)pinMode(i,INPUT_PULLUP);
  readVariables();
}
//=======================================================================================================//



//=================================WATER TANK CONTROL====================================================//

void waterTankSystem(){
    bool isTankLow = !digitalRead(BottomFloatSwitch);
    bool isTankFull = !digitalRead(HighFloatSwtich);
    if(isTankFull)digitalWrite(Relay1,HIGH);
    else if (isTankLow){
      digitalWrite(Relay1,LOW);
      }
  }
  
//=======================================================================================================//




//========================IRRIGATION SYSTEM (SOIL MOISTURE AND RELAY)====================================//

void irrigationSystem(){
    int soilMoisture = getSoilMoisture();
    if(soilMoisture<=(moistureUpperLimit+moistureLowerLimit)/2)digitalWrite(Relay2,HIGH);
    else if(soilMoisture>=((moistureUpperLimit+moistureLowerLimit)/2))digitalWrite(Relay2,LOW);
  }
  
//=======================================================================================================//



//===================================VPD WARNING DETECTION===============================================//

void vpdWarningSystem(){
    float vpd=getVPD();
    if(vpd<=lowerLimitVPD || vpd>=upperLimitVPD){
      digitalWrite(RedLed1,HIGH);
      digitalWrite(RedLed2,HIGH);
  }
  else {
    digitalWrite(RedLed1,LOW);
      digitalWrite(RedLed2,LOW);
    }
  }

//=======================================================================================================//




//================================RESET MILLIS AND SET TO 0==============================================//

void resetMillis(){
      noInterrupts ();
      timer0_millis = 0;
      interrupts ();
  }
  
//=======================================================================================================//




//==================================Save Values to EEPROM================================================//
void saveCalibrate(){
  EEPROM.put(moistureLowerLimitAddress, moistureLowerLimit);
  EEPROM.put(moistureUpperLimitAddress, moistureUpperLimit);
  EEPROM.put(vpdLowerLimitAddress, lowerLimitVPD);
  EEPROM.put(vpdUpperLimitAddress, upperLimitVPD);
  }
//=======================================================================================================//




//==========================================Read From EEPROM=============================================//
void readVariables(){
  EEPROM.get(moistureLowerLimitAddress, moistureLowerLimit);
  EEPROM.get(moistureUpperLimitAddress, moistureUpperLimit);
  EEPROM.get(vpdLowerLimitAddress, lowerLimitVPD);
  EEPROM.get(vpdUpperLimitAddress, upperLimitVPD);

  // if the address read is empty or == -1 it will return these default values
  if(moistureLowerLimit<0)moistureLowerLimit=200;
  if(moistureUppweLimit<0)moistureUppweLimit=400;
  if(lowerLimitVPD<0.0)lowerLimitVPD=0.4;
  if(upperLimitVPD<0.0)upperLimitVPD=1.6;
  }
//=======================================================================================================//

byte buttonPressed(){
  for(int i=3;i<=6;i++)if(!digitalRead(i))return i-2;
  }

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

 void lcdClear(){
  lcdPrint(0,0," ");
  lcdPrint(0,1," ");
  }
  
void updateMenu(){
  if(buttonPressed()==3){
      _delay_ms(200);
      menuState--;
      while(buttonPressed==3);
    }
  if(buttonPressed==4){
    _delay_ms(200);
    menuState++;
    while(buttonPressed==4)
    }
  }

void menuSystem(){
  updateMenu();
  if(menuState>4)menuState=1;
  if(menuState<1)menuState=4;
  switch(menuState){
    case 1: 
    lcdPrint(0,0,"SOIL MOISTURE");
    if(buttonPressed()==2){
      _delay_ms(200);
      calbrateSMS();
      }
    break;
    case 2:
    lcdPrint(0,0,"VPD");
    if(buttonPressed()==2){
      _delay_ms(200);
      editVPD();
      }
    break;
    case 3:
    lcdPrint(0,0,"TEMPERATURE");
    break;
    case 4:
    lcdPrint(0,0,"HUMIDITY");
    break;
    }
  
  }
void calibrateSMS(){
  lcdClear();
  lcdPrint(0,0,"CALIBRATE SMS");
  if(buttonPressed==3){
    soil
    }
  
  }
void editVPD(){
  lcdClear();
  lcdPrint(0,0,"EDIT VPD LIMIT");
  
  
  }
  

void loop() {
waterTankSystem(); 
irrigationSystem();  
vpdWarningSystem();
menuSystem();
}
