#include "../myOS/userInterface.h"
#include "shell.h"

#define WHITE 0x7
extern void tskEnd(void);
extern int myPrintf(int color,const char *format, ...);

char *message1 = "********************************\n";


void myTsk0(void);
void myTsk1(void);
void myTsk2(void);
void myTsk3(void);
void myTsk4(void);

void myTsk0(void){     
	
	createTsk(myTsk3, make_tskPara(1, 10, 1));

	myPrintf(WHITE, message1);
	myPrintf(WHITE, "* Tsk0: tskPara = {2, 2, 2}\n");
	myPrintf(WHITE, message1);
	// printTskStatus();

	tskEnd();   //the task is end
}

void myTsk1(void){
	myPrintf(WHITE, message1);
	myPrintf(WHITE, "* Tsk1: tskPara = {3, 5, 3}\n");
	myPrintf(WHITE, message1);
	// printTskStatus();
	
	tskEnd();   //the task is end
}

void myTsk2(void){
	myPrintf(WHITE, message1);
	myPrintf(WHITE, "* Tsk2: tskPara = {4, 4, 3}\n");
	myPrintf(WHITE, message1);
	// printTskStatus();
	
	tskEnd();  //the task is end
}


void myTsk3(void){
	myPrintf(WHITE, message1);
	myPrintf(WHITE, "* Tsk3: tskPara = {1, 10, 1}\n");
	myPrintf(WHITE, message1);
	// printTskStatus();
	
	tskEnd();  //the task is end
}


void myTsk4(void){
	myPrintf(WHITE, message1);
	myPrintf(WHITE, "* Tsk4: tskPara = {5, 3, 3}\n");
	myPrintf(WHITE, message1);
	// printTskStatus();
	
	tskEnd();  //the task is end
}


int testTskFCFS(int argc, unsigned char **argv){
	myPrintf(WHITE, "testTskFCFS begin......\n");
	
	switchScheduler(0);

	createTsk(myTsk0, make_tskPara(2, 2, 2));
	createTsk(myTsk1, make_tskPara(3, 5, 3));
	createTsk(myTsk2, make_tskPara(4, 4, 3));
	createTsk(myTsk4, make_tskPara(5, 3, 3));
	
	myPrintf(WHITE, "testTskFCFS end......\n");
}

int testTskSJF(int argc, unsigned char **argv){
	myPrintf(WHITE, "testTskSJF begin......\n");
	
	switchScheduler(1);

	createTsk(myTsk0, make_tskPara(2, 2, 2));
	createTsk(myTsk1, make_tskPara(3, 5, 3));
	createTsk(myTsk2, make_tskPara(4, 4, 3));
	createTsk(myTsk4, make_tskPara(5, 3, 3));
	
	myPrintf(WHITE, "testTskSJF end......\n");
}

int testTskPRIO(int argc, unsigned char **argv){
	myPrintf(WHITE, "testTskPRIO begin......\n");
	
	switchScheduler(2);

	createTsk(myTsk0, make_tskPara(2, 2, 2));
	createTsk(myTsk1, make_tskPara(3, 5, 3));
	createTsk(myTsk2, make_tskPara(4, 4, 3));
	createTsk(myTsk4, make_tskPara(5, 3, 3));
	
	myPrintf(WHITE, "testTskPRIO end......\n");
}

void userTasks_init(void){
	addNewCmd("testTskFCFS", testTskFCFS, NULL, "test task schedule: FCFS");
	addNewCmd("testTskSJF", testTskSJF, NULL, "test task schedule: SJF");
	addNewCmd("testTskPRIO", testTskPRIO, NULL, "test task schedule: PRIO");
	addNewCmd("tskStatus", printTskStatus, NULL, "print task queue status");
}