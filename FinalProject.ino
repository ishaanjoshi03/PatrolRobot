#include <IRremote.h>
#include <SR04.h>
#include <LiquidCrystal_I2C.h>

#define FORWARD 0
#define REVERSE 1

// Motor definitions to make life easier:
#define MOTOR_A 0
#define MOTOR_B 1

// Define ultrasonic pins
#define TRIG_PIN 9
#define ECHO_PIN 10

// Pin Assignments //
//Default pins:
#define DIRA 2 // Direction control for motor A
#define PWMA 3  // PWM control (speed) for motor A
#define DIRB 4 // Direction control for motor B
#define PWMB 11 // PWM control (speed) for motor B

const int recvPin = 8;
IRrecv irrecv(recvPin);
decode_results results;
String ir_value;
String ir_value2;

SR04 sr04 = SR04(ECHO_PIN, TRIG_PIN);
long a;

int redLed = 4;
int blueLed = 5;
int buzzer = 6;

LiquidCrystal_I2C lcd(0x27,20,4);



void setup() {
  setupArdumoto();
  Serial.begin(9600);
  irrecv.enableIRIn(); //Starts receiver
  //delay(10000);
  pinMode(redLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
  pinMode(buzzer, OUTPUT);
  lcd.init();
  lcd.backlight();
}

void loop() {
  lcd.setCursor(2, 0);
  lcd.print("1 for Patrol");
  lcd.setCursor(0, 1);
  lcd.print("2 for NightLight");
  // put your main code here, to run repeatedly:
  if (irrecv.decode(&results)) {
  //Serial.println(results.value,HEX);
    ir_value = decodeKeyValue(results.value);
    if (ir_value != "ERROR") {
        Serial.println("irvalue1 = " + ir_value);
        delay(1000);
    }
    irrecv.decode(&results);
    ir_value = decodeKeyValue(results.value);
    Serial.println("irvalue = " + ir_value);
    if (ir_value == "1") {
      //do {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Patrol Mode");
        patrol();
        Serial.println("Here");
          irrecv.resume();
        delay(1000);
    }
  }
    irrecv.decode(&results);
    ir_value = decodeKeyValue(results.value);
    if (ir_value == "2") {
        lcd.clear();
        lcd.print("Night Mode");
        nightlight();
        delay(1000);
        irrecv.resume();
    }
}


String decodeKeyValue(long result)
{
  switch(result){
    case 0xFF6897:
      return "0";
    case 0xFF30CF:
      return "1"; 
    case 0xFF18E7:
      return "2"; 
    case 0xFF7A85:
      return "3"; 
    case 0xFF10EF:
      return "4"; 
    case 0xFF38C7:
      return "5"; 
    case 0xFF5AA5:
      return "6"; 
    case 0xFF42BD:
      return "7"; 
    case 0xFF4AB5:
      return "8"; 
    case 0xFF52AD:
      return "9"; 
    case 0xFF906F:
      return "+"; 
    case 0xFFA857:
      return "-"; 
    case 0xFFE01F:
      return "EQ"; 
    case 0xFFB04F:
      return "U/SD";
    case 0xFF9867:
      return "CYCLE";         
    case 0xFF22DD:
      return "PLAY/PAUSE";   
    case 0xFFC23D:      
      return "FORWARD";   
    case 0xFF02FD:       
      return "BACKWARD";   
    case 0xFFA25D:
      return "POWER";   
    case 0xFFE21D:
      return "MUTE";   
    case 0xFF629D:
      return "MODE";       
    case 0xFFFFFFFF:
      return "ERROR";   
    default :
      return "ERROR";
    }
}

void driveArdumoto(byte motor, byte dir, byte spd) {
  if (motor == MOTOR_A)
  {
    digitalWrite(DIRA, dir);
    analogWrite(PWMA, spd);
  }
  else if (motor == MOTOR_B)
  {
    digitalWrite(DIRB, dir);
    analogWrite(PWMB, spd);
  }  
}

void stopArdumoto(byte motor) {
  driveArdumoto(motor, 0, 0);
}

void setupArdumoto() {
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);

  digitalWrite(PWMA, LOW);
  digitalWrite(PWMB, LOW);
  digitalWrite(DIRA, LOW);
  digitalWrite(DIRB, LOW);
}

void patrol() {
  int speed1 = 70;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Patrol Mode");
  delay(5000);
  for (int i = 0; i < 20; i ++) {
    driveArdumoto(MOTOR_A, FORWARD, speed1);
    driveArdumoto(MOTOR_B, REVERSE, speed1);
    delay(500);
    a = sr04.Distance();
    Serial.print("Distance = ");
    Serial.print(a);
    Serial.println(" cm");
    if (a < 40) {
      stopArdumoto(MOTOR_A);
      stopArdumoto(MOTOR_B);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("ALARM ACTIVATED");
      for (int j = 0; j < 5; j ++) {
        digitalWrite(redLed, HIGH);
        digitalWrite(buzzer, HIGH);
        delay(500);
        digitalWrite(redLed, LOW);
        digitalWrite(buzzer, LOW);
        delay(500);
      }
    }
  }
}

 void nightlight() {
  stopArdumoto(MOTOR_A);
  stopArdumoto(MOTOR_B);
  for (int i = 0; i < 10; i ++) {
    a = sr04.Distance();
    if (a < 30) { 
      digitalWrite(blueLed, HIGH);
      delay(10000);
    }
  }
  
 }



 
