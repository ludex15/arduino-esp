#include <Wire.h> 
#include <LiquidCrystal_I2C.h> 

/* arduino pins */
const int buzzer = 2; //buzzer to arduino pin 9
const int ledC = 4; // red
const int ledZ = 3; // yellow

const int btn1 = 9; // button 1 
const int btn2 = 10; // button 2
/****************/

float voltage_ch1 = 0; //1. measured voltage 
float voltage_ch2 = 0; //2. measured voltage 

/* bools to help with LCD printing */

bool showBothVals = false;
bool showSeparate = false;
bool firstTime = true;
bool lastCH1 = true;
/****************/

int btn1Val; // button 1 state
int btn2Val; // button 2 state

/* debouncing variables */
int buttonState1 = HIGH;   
int buttonState2 = HIGH;  
          
int lastButtonState1 = HIGH;   
int lastButtonState2 = HIGH;   

unsigned long lastDebounceTime1 = 0;  // the last time the output pin was toggled
unsigned long lastDebounceTime2 = 0;  // the last time the output pin was toggled

unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
/****************/

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); // 16x2 LCD. "object" instance

void greetingScreen(){
  lcd.setCursor(0, 0);
  lcd.print("2CH voltmeter");
  long prev = millis();
  while(millis() - prev < 5000){
    
  }
  lcd.clear();
}

void beepOnTouch(){
  tone(buzzer, 1000); // 1KHz
  long prev = millis();
  while(millis() - prev < 100){
    
  }
  noTone(buzzer);     
}

void printCH1(){
  readVoltageCH1();
  lcd.setCursor(0, 0);
  String voltageToPrint = "CH1: ";
  voltageToPrint += String(voltage_ch1);
  voltageToPrint += " V     ";
  lcd.print(voltageToPrint);
  lcd.setCursor(0, 1);
  lcd.print("                ");
}


void printCH2(){
  readVoltageCH2();
  lcd.setCursor(0, 0);
  String voltageToPrint = "CH2: ";
  voltageToPrint += String(voltage_ch2);
  voltageToPrint += " V     ";
  lcd.print(voltageToPrint);
  lcd.setCursor(0, 1);
  lcd.print("                ");
}

void printTogether(){
  readVoltageCH1();
  readVoltageCH2();
  lcd.setCursor(0, 0);
  String voltageToPrint = "CH1: ";
  voltageToPrint += String(voltage_ch1);
  voltageToPrint += " V     ";
  lcd.print(voltageToPrint);

  lcd.setCursor(0, 1);
  voltageToPrint = "CH2: ";
  voltageToPrint += String(voltage_ch2);
  voltageToPrint += " V     ";
  lcd.print(voltageToPrint);
}

void readVoltageCH1(){
  voltage_ch1 = (analogRead(A1)*5)/1024.0;
}

void readVoltageCH2(){
  voltage_ch2 = (analogRead(A2)*5)/1024.0;
}

void setup() {
  
  Serial.begin(9600);
   
  lcd.init();
  lcd.backlight();
  
  pinMode(buzzer, OUTPUT);
  noTone(buzzer);

  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);

  pinMode(ledC, OUTPUT);
  pinMode(ledZ, OUTPUT); 
  digitalWrite(ledC, LOW);
  digitalWrite(ledZ, LOW); 

  greetingScreen();
}

void loop() {

  int reading1 = digitalRead(btn1);
  int reading2 = digitalRead(btn2);

  if (reading1 != lastButtonState1) {
    lastDebounceTime1 = millis();
  }
  else if (reading2 != lastButtonState2) {
    lastDebounceTime2 = millis();
  }
  

  if ((millis() - lastDebounceTime1) > debounceDelay) {
    if (reading1 != buttonState1) {
      buttonState1 = reading1;
      if (buttonState1 == LOW) {
        firstTime = false;
        beepOnTouch();
        showSeparate = false;
        showBothVals = true;
        digitalWrite(ledC,HIGH);
        digitalWrite(ledZ,HIGH);
      }
    }
  }


  if ((millis() - lastDebounceTime2) > debounceDelay) {
    if (reading2 != buttonState2) {
      buttonState2 = reading2;
      if (buttonState2 == LOW) {
        firstTime = false;
        beepOnTouch();
        showSeparate = true;
        showBothVals = false;
        lastCH1 = !lastCH1;
        if(!lastCH1){
          digitalWrite(ledC,HIGH);
          digitalWrite(ledZ,LOW);
        }
        else{
          digitalWrite(ledC,LOW);
          digitalWrite(ledZ,HIGH);
        }
      }
    }
  }

  if(firstTime) {
    lcd.setCursor(0, 0);
    lcd.print("Press any button");
    lcd.setCursor(0, 1);
    lcd.print("to measure......");
  }

  if(showBothVals){
    printTogether();
  }
  else if (showSeparate){
    if(!lastCH1){
      printCH1();
    }
    else{
      printCH2();
    }
  }
  
  lastButtonState1 = reading1;
  lastButtonState2 = reading2;

}
