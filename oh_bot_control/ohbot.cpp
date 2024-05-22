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
	
	//initially set the servos using the rest angle settings for each channel
	for (int i=0; i < NUM_SERVOS; i++) {
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
	
	//initially set the servos using the rest angle settings for each channel
	for (int i=0; i < NUM_SERVOS; i++) {
		servoProps[i].currentAng = servoProps[i].restAng + servoProps[i].offset;
	}

	//for (int j=0; j < NUM_SERVOS; j++) {
	//	pwm.setPWM(servoProps[j].servoNum, 0, angleToPulse(servoProps[j].restAng));
	//}	  	
}

void OhBot::iterateMotion(int now) {
	if (cueIndex < cueListSize) {
		// Cue fires at a specific time
		if (!cues[cueIndex].complete && (now >= cues[cueIndex].timestamp)) {	
	
			++cueIndex;
		}
	}
}
