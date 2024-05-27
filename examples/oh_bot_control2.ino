#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#include <Easing.h>

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

EasingFunc<Ease::QuadOut> qdo;
EasingFunc<Ease::QuadIn> qdi;
EasingFunc<Ease::QuadInOut> qdio;

bool timeInitiated = false; 
int time0;

int currentServoNum = 0;

//servo properties object
typedef struct {
  int servoNum;
  int minAng;
  int maxAng;
  int restAng;
  int offset; //set if servo min < 0 so min angle is never negative
  int currentAng;
} ServoProps;

//properties for each servo 
ServoProps servoProps[NUM_SERVOS] = {
  {0, 0, 120, 55, 0}, //head turn
  {1, 0, 80, 50, -15}, //head nod
  {8, 20, 100, 55, 0}, //eyes horizontal
  {3, 55, 115, 85, 0}, //eyes vertical
  {4, 0, 40, 25, 0}, //eyelids
  {5, 0, 45, 30, -20}, //top lip
  {6, 20, 65, 50, 0}, //bottom lip
};

//named pose list
typedef struct {
  int servoValues[NUM_SERVOS];
} Pose;

#define POSE_LIST_SIZE 1
Pose poses[] = {
  {-1, -1, -1, -1, servoProps[4].maxAng + servoProps[4].offset, -1, -1},//close eyes
};

String poseNameLookup[] = {
  "close eyes",
};

//action list-item object
typedef struct {
  String name;
  int value;
  int timestamp;
  int duration;
  int easing;
  bool complete;
} Action;

#define ACTION_LIST_SIZE 10

//command object
typedef struct {
  String name;
  Action actions[ACTION_LIST_SIZE];
  int timestamp;
} Command;

#define COMMAND_LIST_SIZE 20
Command commands[COMMAND_LIST_SIZE];

//cue object
typedef struct {
  int timestamp;
  String commands[COMMAND_LIST_SIZE];
  bool complete;
  bool started;
} Cue;

#define CUE_LIST_SIZE 10
#define CUE_TIME_WINDOW 300
Cue cues[CUE_LIST_SIZE]; 

void setup() {
  Serial.begin(115200);
  Serial.println("beginning...");

  for (int i=0; i < NUM_SERVOS; i++) {
    servoProps[i].currentAng = servoProps[i].restAng + servoProps[i].offset;
  }

  //command definitions
  commands[0] = {name:"blink", actions:{{"eyelids", -100, 1000, 50}, {"eyelids", 0, 1100, 50}} };
  
  //sample cues
  cues[0] = {timestamp:1000, commands:{"blink"}};
  cues[1] = {timestamp:3000, commands:{"blink"}};
  cues[2] = {timestamp:5000, commands:{"blink"}};
  
  qdo.scale(1);

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

  //initially set the servos using the rest angle settings for each channel
  for (int j=0; j < NUM_SERVOS; j++) {
    pwm.setPWM(servoProps[j].servoNum, 0, angleToPulse(servoProps[j].restAng));
  }

}

void loop() {
  if (!timeInitiated) {
    time0 = millis();
    timeInitiated = true;
  }

  int now = millis() - time0;

  for (int c=0; c < CUE_LIST_SIZE; c++) {
    //Serial.println(now);
    
    //cue fires at specific time
    if (!cues[c].complete && (now >= cues[c].timestamp)) {
      bool allActionsComplete = true;

      //run through command list in cue
      for (int cci=0; cci < COMMAND_LIST_SIZE; cci++) {
        int commandIndex = -1;
        for (int ci=0; ci < COMMAND_LIST_SIZE; ci++) {
          if ((cues[c].commands[cci] != "") && cues[c].commands[cci].equals(commands[ci].name)) {
            commandIndex = ci;
            break;
          }
        }
      
        //if cue command is in command "library", run the actions in the command
        if (commandIndex >= 0) {
          Serial.print(" Command ");
          Serial.print(commandIndex);
          Serial.print(" received ("  + commands[commandIndex].name + ") ");
          Serial.println(now);

          //reset action-complete flags the first time thru the cue
          if (!cues[c].started) {
            for (int i=0; i < ACTION_LIST_SIZE; i++) {
              commands[commandIndex].actions[i].complete = false;
            }

            cues[c].started = true;
          }

          for (int i=0; i < ACTION_LIST_SIZE; i++) {
            if ((commands[commandIndex].actions[i].name != "") && !commands[commandIndex].actions[i].complete) {
              //trigger action if now is greater than timestamp and less than duration 
              //Serial.print(">");
              //Serial.print(actions[i].timestamp);
              //Serial.print(" ");
              if ((now >= commands[commandIndex].actions[i].timestamp + cues[c].timestamp) && 
                  (now <= commands[commandIndex].actions[i].timestamp + commands[commandIndex].actions[i].duration + cues[c].timestamp)
                  ) {
                //find pose in pose list
                int poseIndex = 0;
                for (int j=0; j < POSE_LIST_SIZE; j++) {
                  if (commands[commandIndex].actions[i].name == poseNameLookup[j]) {
                    poseIndex = j;
                    break;
                  }
                }

                Pose pose = poses[poseIndex];
                
                //run action until it is complete
                for (int j=0; j < NUM_SERVOS; j++) {
                  if (pose.servoValues[j] >= 0) {
                    //calculate desired servo angle based on milliseconds ticked
                    // pos value: 100% * (max - rest) / 100 + rest + offset 
                    // neg value: -100% * (rest - min) / 100 + min + offset 
                    int targetAng = commands[commandIndex].actions[i].value >= 0?
                      (commands[commandIndex].actions[i].value * round(servoProps[j].maxAng - servoProps[j].restAng) / 100.) + servoProps[j].restAng + servoProps[j].offset :
                      servoProps[j].restAng + (commands[commandIndex].actions[i].value * round(servoProps[j].restAng - servoProps[j].minAng) / 100.) + servoProps[j].offset;

                    //easing function doesn't work if duration is < 200 ms
                    //so just complete the action;
                    int ang = targetAng;
                    if (commands[commandIndex].actions[i].duration < 200) {
                      servoProps[j].currentAng = targetAng;
                      commands[commandIndex].actions[i].complete = true;
                    } else {
                      //set servo to eased value between current angle and target angle based on action duration
                      //(target - current) * easingFunction + current
                      qdio.duration(commands[commandIndex].actions[i].duration / 1000.);

                      float step = qdio.get((now - commands[commandIndex].actions[i].timestamp - cues[c].timestamp) / 1000.);
                      ang = round((targetAng - servoProps[j].currentAng)*step) + servoProps[j].currentAng;
                      Serial.print((now - commands[commandIndex].actions[i].timestamp - cues[c].timestamp) / 1000.);
                      Serial.print(" - ");

                      //when targetAngle is reached, set currentAng = targetAng;
                      if (ang == targetAng) {
                        servoProps[j].currentAng = targetAng;
                        commands[commandIndex].actions[i].complete = true;

                        Serial.print("cue [");
                        Serial.print(c);
                        Serial.print("] " + commands[commandIndex].actions[i].name + " ");
                        Serial.print(commands[commandIndex].actions[i].value);
                        Serial.print(" complete: ");          
                        printServoValues();
                      }

                      Serial.print(i);
                      Serial.print(". target: ");
                      Serial.print(targetAng);
                      Serial.print("; ang: ");
                      Serial.print(ang);
                      Serial.print("; step: ");
                      Serial.print(step);
                      Serial.print("; ");
                      Serial.println(qdio.get(step));
                    }
                  
                    pwm.setPWM(servoProps[j].servoNum, 0, angleToPulse((int) ang));

                  }
                }
              } else {
                /*Serial.print(i);
                Serial.print(": ");
                Serial.print(now);
                Serial.print("-");
                Serial.println(actions[i].timestamp);//*/
                
              }
            
              allActionsComplete &= commands[commandIndex].actions[i].complete;
            }
          }//*/
        }
      }

      if (allActionsComplete) {
        cues[c].complete = true;
      }
    }
  }

  // check for serial input, values can be:
  if (Serial.available() > 0) {
    char input = Serial.read();

    if (input == 'p') {
      printServoValues();
    }
  }
}

void printServoValues() {
      //Serial.println("nod, shake, eyes-hori, eyes-vert, eyelids, top-lip, bottom-lip");
      Serial.print("{");
      for (int i=0; i < NUM_SERVOS; i++) {
        if (i > 0) {
          Serial.print(", ");
        }
        Serial.print(servoProps[i].currentAng);
      }
      Serial.println("}");

}

int angleToPulse(int ang){
   int pulse = map(ang,0, 180, SERVOMIN,SERVOMAX);// map angle of 0 to 180 to Servo min and Servo max 
  // Serial.print("Angle: ");Serial.print(ang);
  // Serial.print(" pulse: ");Serial.println(pulse);
   return pulse;
}

