int hh, mm, ss, ms;

typedef void HookUpdateFunction(void);
HookUpdateFunction *updateClock_hook = 0;

void tickUpdateClock() {
	// 100HZ -> 10ms/tick
	ms += 10;
	if (ms >= 1000) {
		ms -= 1000;
		++ss;
	}
	if (ss >= 60) {
		ss -= 60;
		++mm;
	}
	if (mm >= 60) {
		mm -= 60;
		++hh;
	}
	if (hh >= 24) {
		hh -= 24;
	}
	if (updateClock_hook) updateClock_hook();
}

void setWallClock(int HH, int MM, int SS){
	if (HH < 0 || HH > 24) hh = 0;
	else hh = HH;
	if (MM < 0 || MM > 60) mm = 0;
	else mm = MM;
	if (SS < 0 || SS > 60) ss = 0;
	else ss = SS;
	if (updateClock_hook) updateClock_hook();
}

void getWallClock(int *HH, int *MM, int *SS){
	*HH = hh;
	*MM = mm;
	*SS = ss;
}

void setClockHook(HookUpdateFunction *newhook){
	updateClock_hook = newhook;
}
