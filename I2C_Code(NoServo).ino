//Import LCD Package for I2C
#include <Wire.h>                  // <-- ADDED: Required for I2C communications
#include "LiquidCrystal_I2C.h"
//important Servo Package

//declaration
const int TRIGpin = 9;
const int ECHOpin = 8;
const int LEDpin = 13;
const int BUZZERpin = 10;
const int Buttonpin = 6;

int safetyThreshold=100; //IMPORTANT
bool longrangeMode=true; //IMPORTANT

// <-- CHANGED: Setup for I2C (Address 0x27, 16 columns, 2 rows)
LiquidCrystal_I2C lcd(0x27, 16, 2); 

//function + display
void setup() {
  pinMode(TRIGpin,OUTPUT);
  pinMode(ECHOpin,INPUT); 
  pinMode(LEDpin,OUTPUT);
  pinMode(BUZZERpin,OUTPUT);
  pinMode(Buttonpin,INPUT_PULLUP);


  Serial.begin(9600);
  

  lcd.begin();          // Wakes up the I2C screen module
  lcd.backlight();     // Turns on the background light

  lcd.print("RADAR ONLINE...");
  delay(1000);
  lcd.clear();
}

//Actual Code
void loop(){
  if(digitalRead(Buttonpin)==LOW){
    longrangeMode=!longrangeMode;
    if(longrangeMode){
      safetyThreshold=100;
    }
    else{
      safetyThreshold=40;
    }
    lcd.clear();
    lcd.print("MODE CHANGED!   ");
    delay(300);
  }
  
    
    digitalWrite(TRIGpin, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGpin,HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGpin,LOW);
  
  long duration = pulseIn(ECHOpin,HIGH);
  int distance = duration * 0.034 / 2;
  
  Serial.print("Space Radar Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

lcd.setCursor(0,0);
  lcd.print("Distance: ");
  lcd.print(distance);
  lcd.print(" cm ");
  lcd.setCursor(0,1);

  if(distance > 0 && distance < safetyThreshold){
    if(distance<safetyThreshold/3){
      tone(BUZZERpin,1000);
      digitalWrite(LEDpin,HIGH);
      lcd.setCursor(0,1);
      lcd.print("CRITICAL IMPACT!");
    }else{
      tone(BUZZERpin,500,50);
      digitalWrite(LEDpin,HIGH);
      lcd.print("ALERT: PROXIMITY");
      int pulseDelay=distance*2;
      delay(pulseDelay);
      
      digitalWrite(LEDpin,LOW);
      delay(pulseDelay);
    }
  }else{
      noTone(BUZZERpin);
      digitalWrite(LEDpin,LOW);
      //lcd.print("PATH: CLEAR     ");
      
      if(longrangeMode){
        lcd.setCursor(0,1);
        lcd.print("MODE: LONG RANGE");
      }else{
        lcd.setCursor(0,1);
        lcd.print("MODE: DOCKING   ");
        
      }
    }
  delay(100);
  
}

