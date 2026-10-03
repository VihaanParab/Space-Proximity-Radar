//Import LCD Package
#include<LiquidCrystal.h>

//declaration
const int TRIGpin = 9;
const int ECHOpin = 8;
const int LEDpin = 13;
const int BUZZERpin = 10;
const int Buttonpin = 6;

int safetyThreshold=100; //IMPORTANT
bool longrangeMode=true; //IMPORTANT

LiquidCrystal lcd(12,11,5,4,3,2);
//function + display
void setup() {
  pinMode(TRIGpin,OUTPUT);
  pinMode(ECHOpin,INPUT); 
  pinMode(LEDpin,OUTPUT);
  pinMode(BUZZERpin,OUTPUT);
  pinMode(Buttonpin,INPUT_PULLUP);

  Serial.begin(9600);
  lcd.begin(16,2);
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

  if(distance<safetyThreshold){
    if(distance<safetyThreshold/3){
      tone(BUZZERpin,1000);
      digitalWrite(LEDpin,HIGH);
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
        lcd.print("MODE: LONG RANGE");
      }else{
        lcd.print("MODE: DOCKING   ");
      }
    }
  delay(50);
}


  
  
     /*if(distance<safetyThreshold){
       digitalWrite(LEDpin,HIGH);
       tone(BUZZERpin,900,50);
       lcd.print("ALERT: PROXIMITY");
       delay(100);
       digitalWrite(LEDpin,LOW);
       delay(100);
     }else{
       digitalWrite(LEDpin,LOW);
       noTone(BUZZERpin);
       lcd.print("PATH CLEAR!     ");
     }
  delay(100);
     }
     
  ANOTHER OPTION FOR CODE BUT NOT ADAPTABLE TO DIFFERENT MODES
  if (distance < 30) {
    digitalWrite(LEDpin,HIGH); //solid red light continuously
    
    tone(BUZZERpin, 1000); //1000 Hz Continuous sound
    lcd.print("CRITICAL IMPACT!");
  } else if (distance >= 30 & distance < 100) {
    digitalWrite(LEDpin,HIGH); //blinking
    tone(BUZZERpin, 600, 50); //medium frequency+duration
    lcd.print("WARNING: CLOSING ");
    delay(100);
    
    digitalWrite(LEDpin,LOW); //blinking
    delay(100);
  } else {
    digitalWrite(LEDpin,LOW); //led off
    noTone(BUZZERpin); //silent buzzer
    lcd.print("PATH CLEAR      ");
  }
  delay(50);
}*/