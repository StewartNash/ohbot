#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#include <Easing.h>

#include "ohbot.hpp"


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

EasingFunc<Ease::QuadOut> qdo;
EasingFunc<Ease::QuadIn> qdi;
EasingFunc<Ease::QuadInOut> qdio;

OhBot myOhBot;

bool timeInitiated = false; 
int time0;

int currentServoNum = 0;

#define POSE_LIST_SIZE 50
Pose poses[] = {
  {"blink",{-1, -1, -1, -1, myOhBot.servoProps[4].minAng, -1, -1}},
  {"turn right",{myOhBot.servoProps[0].minAng, -1, -1, -1, -1, -1, -1}},
  {"turn left",{myOhBot.servoProps[0].maxAng , -1, -1, -1, -1, -1, -1}},
  {"nod up",{-1, myOhBot.servoProps[1].minAng , -1, -1, -1, -1, -1}},
  {"nod down",{-1, myOhBot.servoProps[1].maxAng , -1, -1, -1, -1, -1}},
  {"look up",{-1, -1, -1, myOhBot.servoProps[3].maxAng , -1, -1, -1}},
  {"look down",{-1, -1, -1, myOhBot.servoProps[3].minAng , -1, -1, -1}},
  {"look right",{-1, -1, myOhBot.servoProps[2].minAng, -1 , -1, -1, -1}},
  {"look left",{-1, -1, myOhBot.servoProps[2].maxAng, -1 , -1, -1, -1}},
  {"smile", {-1, -1, -1, -1, -1, myOhBot.servoProps[5].maxAng, 30}},
};

Command commands[COMMAND_LIST_SIZE] = {
  {"blink", {{"blink", 100, 0, 50}, {"blink", 0, 100, 50}} },
  {"look1", {
      {"look right", 30, 0, 500},
      {"look up", 100, 0, 500},
      {"turn right", 54, 550, 300},
      {"nod up", 50, 550, 300},
      {"look right", 15, 550, 300},
      {"look up", 67, 550, 300},
      {"look left", 40, 1500, 50},
      {"look right", 15, 2000, 50},
      {"look left", 40, 2700, 50},
      {"look right", 15, 3500, 50},
      //{"nod up", 60, 4500, 1000},
      {"smile", 100, 4500, 1000},
      {"look up", 30, 4500, 1000},
    }
  }
};

Cue cues[CUE_LIST_SIZE] = {
  {1000, {"blink"}},
  {1000, {"look1"}},
  {3000, {"blink"}},
  {4000, {"blink"}},
  {4500, {"blink"}},
};

void setup() {
	Serial.begin(115200);
  	Serial.println("beginning..."); 
  	
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

	for (int j=0; j < NUM_SERVOS; j++) {
		pwm.setPWM(myOhBot.servoProps[j].servoNum, 0, angleToPulse(myOhBot.servoProps[j].restAng));
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
          if ((strlen(cues[c].commands[cci]) > 0) && (strcmp(cues[c].commands[cci], commands[ci].name) == 0)) {
            commandIndex = ci;
            break;
          }
        }

        //if cue command is in command "library", run the actions in the command
        if (commandIndex >= 0) {
          Serial.print(" Command ");
          Serial.print(commandIndex);
          Serial.print(" received (");
          Serial.print(commands[commandIndex].name);
          Serial.print(") ");

          //reset action-complete flags the first time thru the cue
          if (!cues[c].started) {
            for (int i=0; i < ACTION_LIST_SIZE; i++) {
              commands[commandIndex].actions[i].complete = false;
            }

            cues[c].started = true;
          }

          for (int i=0; i < ACTION_LIST_SIZE; i++) {
            if ((strlen(commands[commandIndex].actions[i].poseName) > 0) && !commands[commandIndex].actions[i].complete) {
              //trigger action if now is greater than timestamp and less than duration 
              //Serial.print(">");
              Serial.print(commands[commandIndex].actions[i].poseName);
              Serial.print(" ");
              Serial.print(now);
              Serial.print(" ");
              Serial.print(commands[commandIndex].actions[i].timestamp + cues[c].timestamp);
              Serial.print("-");
              Serial.print(commands[commandIndex].actions[i].timestamp + commands[commandIndex].actions[i].duration + cues[c].timestamp);
              Serial.print(" ");

              if ((now >= commands[commandIndex].actions[i].timestamp + cues[c].timestamp) && 
                  (now <= commands[commandIndex].actions[i].timestamp + commands[commandIndex].actions[i].duration + cues[c].timestamp)
                 ) {
                //find pose in pose list
                int poseIndex = 0;
                for (int j=0; j < POSE_LIST_SIZE; j++) {
                  if (strcmp(commands[commandIndex].actions[i].poseName, poses[j].name) == 0) {
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
                    int targetAng = pose.servoValues[j] >= myOhBot.servoProps[j].restAng?
                      (commands[commandIndex].actions[i].value * round(pose.servoValues[j] - myOhBot.servoProps[j].restAng) / 100.) + myOhBot.servoProps[j].restAng + myOhBot.servoProps[j].offset :
                      (myOhBot.servoProps[j].restAng - commands[commandIndex].actions[i].value * round(myOhBot.servoProps[j].restAng - pose.servoValues[j]) / 100.) + myOhBot.servoProps[j].offset;

                    //easing function doesn't work if duration is < 200 ms
                    //so just complete the action;
                    int ang = targetAng;
                
                    Serial.print("target:");
                    Serial.print(targetAng);
                    Serial.print(" current:");
                    Serial.print(myOhBot.servoProps[j].currentAng);
                    Serial.print(" ");
                    
                    if (commands[commandIndex].actions[i].duration < 200) {
                      myOhBot.servoProps[j].currentAng = targetAng;
                      commands[commandIndex].actions[i].complete = true;
                      Serial.println(" ");
                    } else {
                      //set servo to eased value between current angle and target angle based on action duration
                      //(target - current) * easingFunction + current
                      qdio.duration(commands[commandIndex].actions[i].duration / 1000.);

                      float step = qdio.get((now - commands[commandIndex].actions[i].timestamp - cues[c].timestamp) / 1000.);
                      ang = round((targetAng - myOhBot.servoProps[j].currentAng)*step) + myOhBot.servoProps[j].currentAng;

                      //when targetAngle is reached, set currentAng = targetAng;
                      if (ang == targetAng) {
                        myOhBot.servoProps[j].currentAng = targetAng;
                        commands[commandIndex].actions[i].complete = true;

                        Serial.print("cue [");
                        Serial.print(c);
                        Serial.print("] ");
                        Serial.print(commands[commandIndex].actions[i].poseName);
                        Serial.print(" ");
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
                  
                    pwm.setPWM(myOhBot.servoProps[j].servoNum, 0, angleToPulse((int) ang));

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
          Serial.println(" ");
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
        Serial.print(myOhBot.servoProps[i].currentAng);
      }
      Serial.println("}");

}

int angleToPulse(int ang){
   int pulse = map(ang, 0, 180, SERVOMIN, SERVOMAX);// map angle of 0 to 180 to Servo min and Servo max 
  // Serial.print("Angle: ");Serial.print(ang);
  // Serial.print(" pulse: ");Serial.println(pulse);
   return pulse;
}

