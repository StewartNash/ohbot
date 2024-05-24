#ifndef OH_BOT_H
#define OH_BOT_H
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#include <Easing.h>


#define NUM_SERVOS 7
#define MAX_NAME_SIZE 12
#define ACTION_LIST_SIZE 20
#define COMMAND_LIST_SIZE 100
#define CUE_LIST_SIZE 20
#define POSE_LIST_SIZE 50

// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates


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
		int commandListSize;
		int actionListSize;
		int poseListSize;
		int numberOfServos;
		
		Pose *poses;
		Command *commands;
		Cue *cues;
				
		EasingFunc<Ease::QuadOut> qdo;
		EasingFunc<Ease::QuadIn> qdi;
		EasingFunc<Ease::QuadInOut> qdio;		
		Adafruit_PWMServoDriver pwm;
		
		void initiatePwm();
		int angleToPulse(int ang);	
	public:
		ServoProps servoProps[NUM_SERVOS]; // Properties for each servo
		
		OhBot();
		OhBot(Pose* poses, Command* commands, Cue* cues);
		void iterateMotion(int now);
		void printServoValues();
		
		void setPoses(Pose* poses);
		void setCommands(Command* commands);
		void setCues(Cue* cues);
		
};

#endif
