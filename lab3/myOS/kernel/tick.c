#include "wallClock.h"
int system_ticks;
int HH,MM,SS;

void tick(void){
	system_ticks++;
	//你需要填写它
	setWallClock(HH,MM,SS);
	return;
}

