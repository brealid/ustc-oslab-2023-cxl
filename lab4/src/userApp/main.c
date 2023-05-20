#include "../myOS/userInterface.h"   //interface from kernel

#include "shell.h"
#include "memTestCase.h"

void wallClock_hook_main(void){
    static int _h, _m, _s;
    static char timeNow[10] = "hh:mm:ss";

    getWallClock(&_h, &_m, &_s);
    sprintf(timeNow, "%02d:%02d:%02d", _h, _m, _s);
    append2specificPosition(timeNow, 0x75, 24, 72);
}

void doSomeTestBefore(void){		
	setWallClock(18,59,59);		//set time 18:59:59
    setWallClockHook(&wallClock_hook_main);
}

void myMain(void){    
    clear_screen();

    doSomeTestBefore();

	initShell();
    memTestCaseInit();
    startShell();
    return;
}
