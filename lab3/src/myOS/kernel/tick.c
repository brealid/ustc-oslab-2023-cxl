#include "wallClock.h"
int system_ticks = 0;

void tick(void) {
	++system_ticks;
	tickUpdateClock();
	return;
}

