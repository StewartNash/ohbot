//Ohbot manual control with 7 pots


#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 60 // Analog servos run at ~50 Hz updates


int Pot1Pin = A0;    // select the input pin for the potentiometer
int Pot1Value = 0;  // potentiometer 1 value
int Pot1Map = 0; //mapped value of pot 1 for servo angle

int Pot2Pin = A1;    // select the input pin for the potentiometer
int Pot2Value = 0;  // potentiometer 2 value
int Pot2Map = 0; //mapped value of pot 2 for servo angle

int Pot3Pin = A2;    // select the input pin for the potentiometer
int Pot3Value = 0;  // potentiometer 3 value
int Pot3Map = 0; //mapped value of pot 3 for servo angle

int Pot4Pin = A3;    // select the input pin for the potentiometer
int Pot4Value = 0;  // potentiometer 4 value
int Pot4Map = 0; //mapped value of pot 4 for servo angle

int Pot5Pin = A4;    // select the input pin for the potentiometer
int Pot5Value = 0;  // potentiometer 5 value
int Pot5Map = 0; //mapped value of pot 5 for servo angle

int Pot6Pin = A5;    // select the input pin for the potentiometer
int Pot6Value = 0;  // potentiometer 6 value
int Pot6Map = 0; //mapped value of pot 6 for servo angle

int Pot7Pin = A6;    // select the input pin for the potentiometer
int Pot7Value = 0;  // potentiometer 7 value
int Pot7Map = 0; //mapped value of pot 7 for servo angle


void setup() {
  Serial.begin(9600);
  Serial.println("Ohbot control");

  pwm.begin();
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates

  delay(10);
}

void loop() {
 
 //Head Turn
 // read the value from the Pots:
  Pot1Value = analogRead(Pot1Pin);
  Pot1Map = map(Pot1Value, 0, 1024, 0, 180);
  Pot1Map = constrain(Pot1Map, 40, 140);
  pwm.setPWM(0, 0, angleToPulse(Pot1Map));
 // Serial.print(Pot1Value);
 // Serial.print("\t");
 // Serial.println(Pot1Map);

// Nod
  Pot2Value = analogRead(Pot2Pin);
  Pot2Map = map(Pot2Value, 1, 1024, 0, 180);
  Pot2Map = constrain(Pot2Map, 60, 125);
  pwm.setPWM(1, 0, angleToPulse(Pot2Map));
  //Serial.print(Pot2Value);
  //Serial.print("\t");
  //Serial.println(Pot2Map);

// Eyes Left / Right
  Pot3Value = analogRead(Pot3Pin);
  Pot3Map = map(Pot3Value, 1, 1024, 0, 180);
  Pot3Map = constrain(Pot3Map, 60, 145);
  pwm.setPWM(2, 0, angleToPulse(Pot3Map));
  //Serial.print(Pot3Value);
  //Serial.print("\t");
  //Serial.println(Pot3Map);

//Eyes Up / Down
  Pot4Value = analogRead(Pot4Pin);
  Pot4Map = map(Pot4Value, 1, 1024, 0, 180);
  Pot4Map = constrain(Pot4Map, 80, 140);
  pwm.setPWM(3, 0, angleToPulse(Pot4Map));
  //Serial.print(Pot4Value);
  //Serial.print("\t");
  //Serial.println(Pot4Map);

// Eyelids
  Pot5Value = analogRead(Pot5Pin);
  Pot5Map = map(Pot5Value, 1, 1024, 0, 180);
  Pot5Map = constrain(Pot5Map, 0, 50);
  pwm.setPWM(4, 0, angleToPulse(Pot5Map));
 // Serial.print(Pot5Value);
  //Serial.print("\t");
 // Serial.println(Pot5Map);

// Top Lip
  Pot6Value = analogRead(Pot6Pin);
  Pot6Map = map(Pot6Value, 1, 1024, 0, 180);
  Pot6Map = constrain(Pot6Map, 35, 75);
  pwm.setPWM(8, 0, angleToPulse(Pot6Map));
  //Serial.print(Pot6Value);
  //Serial.print("\t");
  //Serial.println(Pot6Map);

  Pot7Value = analogRead(Pot7Pin);
  Pot7Map = map(Pot7Value, 1, 1024, 0, 180);
  Pot7Map = constrain(Pot7Map, 30, 80);
  pwm.setPWM(12, 0, angleToPulse(Pot7Map));
  //Serial.print(Pot7Value);
  //Serial.print("\t");
  //Serial.println(Pot7Map);


 /* 
 Serial.print(Pot1Value);
 Serial.print("\t");
 Serial.print(Pot2Value);
 Serial.print("\t");
 Serial.print(Pot3Value);
 Serial.print("\t");
 Serial.print(Pot4Value);
 Serial.print("\t");
 Serial.print(Pot5Value);
 Serial.print("\t");
 Serial.print(Pot6Value);
 Serial.print("\t");
 Serial.println(Pot7Value);
 */

  delay(50);
}


int angleToPulse(int ang){
   int pulse = map(ang,0, 180, SERVOMIN,SERVOMAX);// map angle of 0 to 180 to Servo min and Servo max 
  // Serial.print("Angle: ");Serial.print(ang);
  // Serial.print(" pulse: ");Serial.println(pulse);
   return pulse;
}
 
