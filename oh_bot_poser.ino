#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
// you can also call it with a different address and I2C interface
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);

// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

#define NUM_SERVOS 7
int currentServoNum = 0;

typedef struct {
  int servoNum;
  int minAng;
  int maxAng;
  int restAng;
  int currentAng;
} ServoProps;

//properties for each servo 
ServoProps servoProps[NUM_SERVOS] = {
  {0, 0, 120, 55}, //head turn
  {1, -15, 65, 35}, //head nod
  {8, 20, 100, 55}, //eyes horizontal
  {3, 55, 115, 85}, //eyes vertical
  {4, 0, 40, 25}, //eyelids
  {5, -20, 15, 15}, //top lip
  {6, 20, 65, 65}, //bottom lip
};

void setup() {
  Serial.begin(9600);
  Serial.println("8 channel Servo test!");

  for (int i=0; i < NUM_SERVOS; i++) {
    servoProps[i].currentAng = servoProps[i].restAng;
  }

  pwm.begin();

  /*
   * In theory the internal oscillator (clock) is 25MHz but it really isn't
   * that precise. You can 'calibrate' this by tweaking this number until
   * you get the PWM update frequency you're expecting!
   * The int.osc. for the PCA9685 chip is a range between about 23-27MHz and
   * is used for calculating things like writeMicroseconds()
   * Analog servos run at ~50 Hz updates, It is importaint to use an
   * oscilloscope in setting the int.osc frequency for the I2C PCA9685 chip.
   * 1) Attach the oscilloscope to one of the PWM signal pins and ground on
   *    the I2C PCA9685 chip you are setting the value for.
   * 2) Adjust setOscillatorFrequency() until the PWM update frequency is the
   *    expected value (50Hz for most ESCs)
   * Setting the value here is specific to each individual I2C PCA9685 chip and
   * affects the calculations for the PWM update frequency. 
   * Failure to correctly set the int.osc value will cause unexpected PWM results
   */
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates

  delay(10);
}

int extra = 0;

void loop() {
  //pose the servos using the current angle settings for each channel
  for (int j=0; j < NUM_SERVOS; j++) {
    pwm.setPWM(servoProps[j].servoNum, 0, angleToPulse(servoProps[j].currentAng));
  }

  // check for serial input, values can be:
  // 0 thru 7 to change servo channel
  // + or - to change servo angle
  // p to print out current pose values
  if (Serial.available() > 0) {
    char input = Serial.read();

    if ((input == '=') || (input == '+')) {
      servoProps[currentServoNum].currentAng += 5;

      if (servoProps[currentServoNum].currentAng > servoProps[currentServoNum].maxAng) {
        servoProps[currentServoNum].currentAng = servoProps[currentServoNum].maxAng;
      }
    } else if (input == '-') {
      servoProps[currentServoNum].currentAng -= 5;

      if (servoProps[currentServoNum].currentAng < servoProps[currentServoNum].minAng) {
        servoProps[currentServoNum].currentAng = servoProps[currentServoNum].minAng;
      }
    } else  if (input == 'p') {
      Serial.println("nod, shake, eyes-hori, eyes-vert, eyelids, top-lip, bottom-lip");
      Serial.print("{");
      for (int i=0; i < NUM_SERVOS; i++) {
        if (i > 0) {
          Serial.print(", ");
        }
        Serial.print(servoProps[i].currentAng);
      }
      Serial.println("}");
    } else {
      currentServoNum = input - '0';
    }

    if (input != 'p') {
      Serial.print(currentServoNum);
      Serial.print(": ");
      Serial.println(servoProps[currentServoNum].currentAng);
    }

  }
 
}

int angleToPulse(int ang){
   int pulse = map(ang,0, 180, SERVOMIN,SERVOMAX);// map angle of 0 to 180 to Servo min and Servo max 
  // Serial.print("Angle: ");Serial.print(ang);
  // Serial.print(" pulse: ");Serial.println(pulse);
   return pulse;
}

