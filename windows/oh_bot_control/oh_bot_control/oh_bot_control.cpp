/*
 * ServoProps {
 * 	(1) servo number,
 * 	(2) min angle,
 * 	(3) max angle,
 * 	(4) rest angle,
 * 	(5) offset,
 * 	(6) current angle
 * }
 * 
 * Pose {
 * 	(1) name,
 * 	(2) servo values {
 * 		(1) servo 1,
 * 		(2) servo 2,
 * 		(3) servo 3, . . .
 *   }
 * }
 * 
 * Action {
 * 	(1) pose name,
 * 	(2) value?,
 * 	(3) timestamp,
 * 	(4) duration,
 * 	(5) easing,
 * 	(6) completion
 * }
 * 
 * Command {
 * 	(1) name,
 * 	(2) action array,
 * 	(3) timestamp
 * }
 * 
 * Cue {
 * 	(1) timestamp,
 * 	(2) command matrix,
 * 	(3) completion,
 * 	(4) started
 * }
 */
#include "ohbot.hpp"

#ifdef VISUAL_STUDIO_DEBUG
void setup();
void loop();

int main() {
	bool exit = false;
	
	setup();

	while (!exit) {
		loop();
	}
}
#endif

OhBot myOhBot;

bool timeInitiated = false; 
int time0;

int currentServoNum = 0;

Pose poses[] = {
	{"blink", {-1, -1, -1, -1, myOhBot.servoProps[4].minAng, -1, -1}},
	{"turn right", {myOhBot.servoProps[0].minAng, -1, -1, -1, -1, -1, -1}},
	{"turn left", {myOhBot.servoProps[0].maxAng , -1, -1, -1, -1, -1, -1}},
	{"nod up", {-1, myOhBot.servoProps[1].minAng , -1, -1, -1, -1, -1}},
	{"nod down", {-1, myOhBot.servoProps[1].maxAng , -1, -1, -1, -1, -1}},
	{"look up", {-1, -1, -1, myOhBot.servoProps[3].maxAng , -1, -1, -1}},
	{"look down", {-1, -1, -1, myOhBot.servoProps[3].minAng , -1, -1, -1}},
	{"look right", {-1, -1, myOhBot.servoProps[2].minAng, -1 , -1, -1, -1}},
	{"look left", {-1, -1, myOhBot.servoProps[2].maxAng, -1 , -1, -1, -1}},
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
  	
  	myOhBot.setPoses(poses);
  	myOhBot.setCommands(commands);
  	myOhBot.setCues(cues);  	
}

void loop() {
  if (!timeInitiated) {
    time0 = millis();
    timeInitiated = true;
  }

  int now = millis() - time0;
  
  myOhBot.iterateMotion(now);

  // check for serial input, values can be:
  if (Serial.available() > 0) {
    char input = Serial.read();

    if (input == 'p') {
      myOhBot.printServoValues();
    }
  }
}
