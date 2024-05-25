#include "ohbot.hpp"

OhBot::OhBot() : servoProps{
	{0, 0, 120, 55, 0}, //head turn
	{1, 0, 80, 50, -15}, //head nod
	{8, 20, 100, 55, 0}, //eyes horizontal
	{3, 35, 115, 55, 0}, //eyes vertical
	{4, 0, 40, 25, 0}, //eyelids
	{5, 0, 45, 35, -20}, //top lip
	{6, 20, 65, 50, 0}, //bottom lip
	}
{
	cueIndex = 0;
	cueListSize = CUE_LIST_SIZE;
	commandListSize = COMMAND_LIST_SIZE;
	actionListSize = ACTION_LIST_SIZE;
	poseListSize = POSE_LIST_SIZE;
	numberOfServos = NUM_SERVOS;
	
	// Called this way, it uses the default address 0x40
	pwm = Adafruit_PWMServoDriver();
	// You can also call it with a different address you want
	//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
	// You can also call it with a different address and I2C interface
	//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);

	qdo.scale(1);	
  	pwm.begin();

	// Initially set the servos using the rest angle settings for each channel
	for (int i=0; i < numberOfServos; i++) {
		servoProps[i].currentAng = servoProps[i].restAng + servoProps[i].offset;
	}

	//for (int j=0; j < NUM_SERVOS; j++) {
	//	pwm.setPWM(servoProps[j].servoNum, 0, angleToPulse(servoProps[j].restAng));
	//}	  	
}

		

OhBot::OhBot(Pose* poses, Command* commands, Cue* cues) : servoProps{
	{0, 0, 120, 55, 0}, //head turn
	{1, 0, 80, 50, -15}, //head nod
	{8, 20, 100, 55, 0}, //eyes horizontal
	{3, 35, 115, 55, 0}, //eyes vertical
	{4, 0, 40, 25, 0}, //eyelids
	{5, 0, 45, 35, -20}, //top lip
	{6, 20, 65, 50, 0}, //bottom lip
	}, poses(poses), commands(commands), cues(cues)
{
	cueIndex = 0;
	cueListSize = CUE_LIST_SIZE;
	commandListSize = COMMAND_LIST_SIZE;
	actionListSize = ACTION_LIST_SIZE;
	poseListSize = POSE_LIST_SIZE;
	numberOfServos = NUM_SERVOS;
	
	// Called this way, it uses the default address 0x40
	pwm = Adafruit_PWMServoDriver();
	// You can also call it with a different address you want
	//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
	// You can also call it with a different address and I2C interface
	//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);

	qdo.scale(1);	
  	pwm.begin();

	// Initially set the servos using the rest angle settings for each channel
	for (int i=0; i < numberOfServos; i++) {
		servoProps[i].currentAng = servoProps[i].restAng + servoProps[i].offset;
	}

	//for (int j=0; j < NUM_SERVOS; j++) {
	//	pwm.setPWM(servoProps[j].servoNum, 0, angleToPulse(servoProps[j].restAng));
	//}	  	
}

int OhBot::angleToPulse(int ang) {
   int pulse = map(ang, 0, 180, SERVOMIN, SERVOMAX);// Map angle of 0 to 180 to Servo min and Servo max 
  // Serial.print("Angle: ");Serial.print(ang);
  // Serial.print(" pulse: ");Serial.println(pulse);
   return pulse;
}

void OhBot::initiatePwm() {
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
	for (int j = 0; j < numberOfServos; j++) {
		pwm.setPWM(servoProps[j].servoNum, 0, angleToPulse(servoProps[j].restAng));
	} 	
}

void OhBot::iterateMotion(int now) {
	// cueIndex <-- c
	// commandIndex <-- cci
	// commandNumber <-- commandIndex 
	for (cueIndex = 0; cueIndex < cueListSize; cueIndex++) {
		// Cue fires at a specific time
		if (!cues[cueIndex].complete && (now >= cues[cueIndex].timestamp)) {
			bool allActionsComplete = true;
			// Run through command list in cue
			for (int commandIndex = 0; commandIndex < commandListSize; commandIndex++) {
				int commandNumber = -1;
				for (int i = 0; i < commandListSize; i++) {
					if ((strlen(cues[cueIndex].commands[commandIndex]) > 0) && (strcmp(cues[cueIndex].commands[commandIndex], commands[i].name) == 0)) {
						commandNumber = i;
						break;
					}
				}
				
				// If cue command is in the command library, run the actions in the command
				if (commandNumber >= 0) {
					Serial.print(" Command ");
					Serial.print(commandNumber);
					Serial.print(" received (");
					Serial.print(commands[commandNumber].name);
					Serial.print(") ");
					
					// Reset action-complete flags the first time through the cue
					if (!cues[cueIndex].started) {
						for (int i = 0; i < actionListSize; i++) {
            
						}
					
					}
					
					for (int i=0; i < actionListSize; i++) {
						if ((strlen(commands[commandNumber].actions[i].poseName) > 0) && !commands[commandNumber].actions[i].complete) {
							// Trigger action if now is greater than timestamp and less than duration 
							//Serial.print(">");
							Serial.print(commands[commandNumber].actions[i].poseName);
							Serial.print(" ");
							Serial.print(now);
							Serial.print(" ");
							Serial.print(commands[commandNumber].actions[i].timestamp + cues[cueIndex].timestamp);
							Serial.print("-");
							Serial.print(commands[commandNumber].actions[i].timestamp + commands[commandNumber].actions[i].duration + cues[cueIndex].timestamp);
							Serial.print(" ");

							if ((now >= commands[commandNumber].actions[i].timestamp + cues[cueIndex].timestamp) && 
							(now <= commands[commandNumber].actions[i].timestamp + commands[commandNumber].actions[i].duration + cues[cueIndex].timestamp)
							) {
								// Find pose in pose list
								int poseIndex = 0;
								for (int j = 0; j < poseListSize; j++) {
									if (strcmp(commands[commandNumber].actions[i].poseName, poses[j].name) == 0) {
										poseIndex = j;
										break;
									}
								}

								Pose pose = poses[poseIndex];

								// Run action until it is complete
								for (int j = 0; j < numberOfServos; j++) {
									if (pose.servoValues[j] >= 0) {
										// Calculate desired servo angle based on milliseconds ticked
										// pos value: 100% * (max - rest) / 100 + rest + offset 
										// neg value: -100% * (rest - min) / 100 + min + offset 
										int targetAng = pose.servoValues[j] >= servoProps[j].restAng?
										(commands[commandNumber].actions[i].value * round(pose.servoValues[j] - servoProps[j].restAng) / 100.) + servoProps[j].restAng + servoProps[j].offset :
										(servoProps[j].restAng - commands[commandNumber].actions[i].value * round(servoProps[j].restAng - pose.servoValues[j]) / 100.) + servoProps[j].offset;

										// Easing function doesn't work if duration is < 200 ms
										// so just complete the action;
										int ang = targetAng;

										Serial.print("target:");
										Serial.print(targetAng);
										Serial.print(" current:");
										Serial.print(servoProps[j].currentAng);
										Serial.print(" ");

										if (commands[commandNumber].actions[i].duration < 200) {
											servoProps[j].currentAng = targetAng;
											commands[commandNumber].actions[i].complete = true;
											Serial.println(" ");
										} else {
											// Set servo to eased value between current angle and target angle based on action duration
											//(target - current) * easingFunction + current
											qdio.duration(commands[commandNumber].actions[i].duration / 1000.);

											float step = qdio.get((now - commands[commandNumber].actions[i].timestamp - cues[cueIndex].timestamp) / 1000.);
											ang = round((targetAng - servoProps[j].currentAng) * step) + servoProps[j].currentAng;

											// When targetAngle is reached, set currentAng = targetAng;
											if (ang == targetAng) {
												servoProps[j].currentAng = targetAng;
												commands[commandNumber].actions[i].complete = true;

												Serial.print("cue [");
												Serial.print(cueIndex);
												Serial.print("] ");
												Serial.print(commands[commandNumber].actions[i].poseName);
												Serial.print(" ");
												Serial.print(commands[commandNumber].actions[i].value);
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

							allActionsComplete &= commands[commandNumber].actions[i].complete;
						}
					}
					Serial.println(" ");									
				}				
			}
		}
	}
}

void OhBot::printServoValues() {
      //Serial.println("nod, shake, eyes-hori, eyes-vert, eyelids, top-lip, bottom-lip");
      Serial.print("{");
      for (int i = 0; i < numberOfServos; i++) {
        if (i > 0) {
          Serial.print(", ");
        }
        Serial.print(servoProps[i].currentAng);
      }
      Serial.println("}");
}

void OhBot::setPoses(Pose* poses) {
	this->poses = poses;
}

void OhBot::setCommands(Command* commands) {
	this->commands = commands;
}

void OhBot::setCues(Cue* cues) {
	this->cues = cues;
}
