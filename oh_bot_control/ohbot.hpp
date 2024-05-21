#ifndef OH_BOT_H
#define OH_BOT_H
#include <Adafruit_PWMServoDriver.h>


#define NUM_SERVOS 7
#define MAX_NAME_SIZE 12
#define ACTION_LIST_SIZE 20
#define COMMAND_LIST_SIZE 100


//servo properties object
typedef struct {
  int servoNum;
  int minAng;
  int maxAng;
  int restAng;
  int offset; //set if servo min < 0 so min angle is never negative
  int currentAng;
} ServoProps;

//named pose list
typedef struct {
  char name[MAX_NAME_SIZE];
  int servoValues[NUM_SERVOS];
} Pose;

//action list-item object
typedef struct {
  char poseName[MAX_NAME_SIZE];
  int value;
  int timestamp;
  int duration;
  int easing;
  bool complete;
} Action;

//command object
typedef struct {
  char name[MAX_NAME_SIZE];
  Action actions[ACTION_LIST_SIZE];
  int timestamp;
} Command;

//cue object
typedef struct {
  int timestamp;
  char commands[COMMAND_LIST_SIZE][MAX_NAME_SIZE];
  bool complete;
  bool started;
} Cue;

class OhBot {		 
	public:
		ServoProps servoProps[NUM_SERVOS]//properties for each servo
		OhBot() : servoProps{
  			{0, 0, 120, 55, 0}, //head turn
  			{1, 0, 80, 50, -15}, //head nod
  			{8, 20, 100, 55, 0}, //eyes horizontal
  			{3, 35, 115, 55, 0}, //eyes vertical
  			{4, 0, 40, 25, 0}, //eyelids
  			{5, 0, 45, 35, -20}, //top lip
  			{6, 20, 65, 50, 0}, //bottom lip
			}
		{
			//initially set the servos using the rest angle settings for each channel
			for (int i=0; i < NUM_SERVOS; i++) {
				servoProps[i].currentAng = servoProps[i].restAng + servoProps[i].offset;
	  		}
	  	
			//for (int j=0; j < NUM_SERVOS; j++) {
			//	pwm.setPWM(servoProps[j].servoNum, 0, angleToPulse(servoProps[j].restAng));
			//}	  	
		}
};

#endif
