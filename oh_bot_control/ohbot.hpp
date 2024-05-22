#ifndef OH_BOT_H
#define OH_BOT_H
#include <Adafruit_PWMServoDriver.h>


#define NUM_SERVOS 7
#define MAX_NAME_SIZE 12
#define ACTION_LIST_SIZE 20
#define COMMAND_LIST_SIZE 100
#define CUE_LIST_SIZE 20


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
		int cueIndex;
		int cueListSize;
		Pose *poses;
		Command *commands;
		Cue *cues;		
	public:
		ServoProps servoProps[NUM_SERVOS]; //properties for each servo
		
		OhBot();
		OhBot(Pose* poses, Command* commands, Cue* cues);
		void iterateMotion(int now);	
		
};

#endif
