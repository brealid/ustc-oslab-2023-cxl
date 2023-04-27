#ifndef __WALLCLOCK_H__
#define __WALLCLOCK_H__

typedef void HookUpdateFunction(void);

void tickUpdateClock();
void setWallClock(int HH,int MM,int SS);
void getWallClock(int *HH,int *MM,int *SS);
void setClockHook(HookUpdateFunction *newhook);

#endif
