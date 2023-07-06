#ifndef __TASK_H__
#define __TASK_H__

#ifndef USER_TASK_NUM
#include "../../userApp/userApp.h"
#endif

#define TASK_NUM (2 + USER_TASK_NUM) // at least: 0-idle, 1-init

#define STACK_SIZE 512 // definition of STACK_SIZE

void CTX_SW();

typedef struct tskPara {
     int priority;  // 优先级
     int exeTime;   // 执行时间
     int arrTime;   // 到达时间
} tskPara;

typedef struct myTCB {
     unsigned long *stkTop;           // 栈顶指针
     unsigned long stack[STACK_SIZE]; // 开辟了一个大小为STACK_SIZE的栈空间
     unsigned long TSK_State;         // 进程状态
     unsigned long TSK_ID;            // 进程ID
     void (*task_entrance)(void);     // 进程的入口地址
     struct myTCB *nextTCB;           // 下一个TCB
     int priority;                    // 优先级
} myTCB;

// typedef struct scheduler
// {
//      unsigned long type;    // the type of the scheduler
//      int preemptive_or_not; // if True, the scheduler is preemptive
//      myTCB *(*nextTsk_func)(void);
//      void (*enqueueTsk_func)(myTCB *tsk);
//      void (*dequeueTsk_func)(myTCB *tsk);
//      void (*schedulerInit_func)(myTCB *idleTsk);
//      int (*createTsk_hook)(void (*tskBody)(void), tskPara para);
//      void (*tick_hook)(void); // if set, tick_hook will be called every tick
// } scheduler;

myTCB tcbPool[TASK_NUM]; // 进程池的大小设置

myTCB *idleTsk;      // idle 任务
myTCB *currentTsk;   // 当前任务
myTCB *firstFreeTsk; // 下一个空闲的 TCB
myTCB *readyQueue_head, *readyQueue_tail;   // 就绪队列

#define NULL ((void*)0)
#define TSK_RDY 0        //表示当前进程已经进入就绪队列中
#define TSK_WAIT -1      //表示当前进程还未进入就绪队列中
#define TSK_RUNING 1     //表示当前进程正在运行
#define TSK_NONE 2       //表示进程池中的TCB为空未进行分配

void TaskManagerInit(void (*initTskBody)(void));

tskPara make_tskPara(int arrTime, int exeTime, int priority);
int createTsk(void (*tskBody)(void), tskPara para);
void switchScheduler(int scheduler);
void printTskStatus(void);

#endif
