#include "../myOS/userInterface.h"   //interface from kernel
#include "shell.h"
#include "memTestCase.h"

void startShell(void);
void initShell(void);
void memTestCaseInit(void);
void userTasks_init(void);
void tskEnd(void);

void wallClock_hook_main(void){
	int _h, _m, _s;
	char hhmmss[]="hh:mm:ss\0\0\0\0";

	getWallClock(&_h,&_m,&_s);
	sprintf(hhmmss,"%02d:%02d:%02d",_h,_m,_s);
	put_chars(hhmmss,0x7E,24,72);
}

void doSomeTestBefore(void){		
	setWallClock(18,59,59);		//set time 18:59:59
    	setWallClockHook(&wallClock_hook_main);
}

// init task 入口
void myMain(void) {
     clear_screen();

     //myPrintf(0x2,"debug_myMain_begin......\n");

     doSomeTestBefore();
     
     myPrintf(0x07, "********************************\n");
	myPrintf(0x07, "*         INIT   INIT !        *\n");
	myPrintf(0x07, "********************************\n");

     //#error "TODO: 初始化 shell 并创建 shell task"
     
     initShell();
     memTestCaseInit();
     userTasks_init();
     createTsk(startShell, make_tskPara(128, 128, 128));

     //myPrintf(0x2,"debug_myMain_end......\n");

     tskEnd();
}
