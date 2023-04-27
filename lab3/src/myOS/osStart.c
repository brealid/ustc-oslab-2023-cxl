#include "myPrintk.h"
#include "vga.h"
#include "i8253.h"
#include "i8259A.h"
#include "tick.h"
#include "wallClock.h"
#include "vsprintf.h"
void myMain(void);
void startShell(void);

void wallClockHook_main() {
    static int _h, _m, _s;
    static char timeNow[10] = "hh:mm:ss";

    getWallClock(&_h, &_m, &_s);
    sprintf(timeNow, "%02d:%02d:%02d", _h, _m, _s);
    append2specificPosition(timeNow, 0x75, 24, 72);
}

void osStart(void)
{
	init8259A();
	init8253();
	enable_interrupt();
    setWallClock(00, 00, 00);
    setClockHook(wallClockHook_main);
    clear_screen();
	
    myPrintk(0x07, "System MyOS.oslab-3 Started\n\n");
    myMain();
    myPrintk(0x07, "System Shutdown\n");
    while (1);
}
