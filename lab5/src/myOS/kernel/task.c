#include "../include/task.h"
#include "../include/myPrintk.h"

void schedule(void);
void destroyTsk(int takIndex);

#define NULL ((void*)0)
#define TSK_RDY 0        //表示当前进程已经进入就绪队列中
#define TSK_WAIT -1      //表示当前进程还未进入就绪队列中
#define TSK_RUNING 1     //表示当前进程正在运行
#define TSK_NONE 2       //表示进程池中的TCB为空未进行分配

//tskIdleBdy进程（无需填写）
void tskIdleBdy(void) {
     while(1){
          schedule();
     }
}

//tskEmpty进程（无需填写）
void tskEmpty(void){
}

//就绪队列的结构体
typedef struct rdyQueueFCFS{
     myTCB * head;
     myTCB * tail;
     myTCB * idleTsk;
} rdyQueueFCFS;

rdyQueueFCFS rqFCFS;

//初始化就绪队列（需要填写）
void rqFCFSInit(myTCB* idleTsk) {//对rqFCFS进行初始化处理
     rqFCFS.head = rqFCFS.tail = NULL;
     rqFCFS.idleTsk = idleTsk;
}

//如果就绪队列为空，返回True（需要填写）
int rqFCFSIsEmpty(void) {//当head和tail均为NULL时，rqFCFS为空
     return (rqFCFS.head == NULL && rqFCFS.tail == NULL);
}

//获取就绪队列的头结点信息，并返回（需要填写）
myTCB * nextFCFSTsk(void) {//获取下一个Tsk
     if(rqFCFSIsEmpty()){
          return rqFCFS.idleTsk;
     }
     else{
          return rqFCFS.head;
     }
}

//将一个未在就绪队列中的TCB加入到就绪队列中（需要填写）
void tskEnqueueFCFS(myTCB *tsk) {//将tsk入队rqFCFS
     tsk->nextTCB = NULL;
     if(rqFCFSIsEmpty()){
          rqFCFS.head = rqFCFS.tail = tsk;
     }
     else{
          rqFCFS.tail->nextTCB = tsk;
          rqFCFS.tail = tsk;
     }
}

//将就绪队列中的TCB移除（需要填写）
void tskDequeueFCFS(myTCB *tsk) {//rqFCFS出队
     if(rqFCFSIsEmpty()){
          return;
     }
     else{
          if(rqFCFS.head == rqFCFS.tail){
               rqFCFS.head = rqFCFS.tail = NULL;
          }
          else{
               rqFCFS.head = rqFCFS.head->nextTCB;
          }
     }
}

//初始化栈空间（不需要填写）
void stack_init(unsigned long **stk, void (*task)(void)){
     *(*stk)-- = (unsigned long) 0x08;       //高地址
     *(*stk)-- = (unsigned long) task;       //EIP
     *(*stk)-- = (unsigned long) 0x0202;     //FLAG寄存器

     *(*stk)-- = (unsigned long) 0xAAAAAAAA; //EAX
     *(*stk)-- = (unsigned long) 0xCCCCCCCC; //ECX
     *(*stk)-- = (unsigned long) 0xDDDDDDDD; //EDX
     *(*stk)-- = (unsigned long) 0xBBBBBBBB; //EBX

     *(*stk)-- = (unsigned long) 0x44444444; //ESP
     *(*stk)-- = (unsigned long) 0x55555555; //EBP
     *(*stk)-- = (unsigned long) 0x66666666; //ESI
     *(*stk)   = (unsigned long) 0x77777777; //EDI

}

//进程池中一个未在就绪队列中的TCB的开始（不需要填写）
void tskStart(myTCB *tsk){
     tsk->TSK_State = TSK_RDY;
     //将一个未在就绪队列中的TCB加入到就绪队列
     tskEnqueueFCFS(tsk);
}

//进程池中一个在就绪队列中的TCB的结束（不需要填写）
void tskEnd(void){
     //将一个在就绪队列中的TCB移除就绪队列
     tskDequeueFCFS(currentTsk);
     //由于TCB结束，我们将进程池中对应的TCB也删除
     destroyTsk(currentTsk->TSK_ID);
     //TCB结束后，我们需要进行一次调度
     schedule();
}

//以tskBody为参数在进程池中创建一个进程，并调用tskStart函数，将其加入就绪队列（需要填写）
int createTsk(void (*tskBody)(void)){//在进程池中创建一个进程，并把该进程加入到rqFCFS队列中
     myTCB * thisTCB;
     if(firstFreeTsk == NULL){
          return -1;
     }
     else{
          thisTCB = firstFreeTsk;
          firstFreeTsk = firstFreeTsk->nextTCB;
          thisTCB->TSK_State = TSK_WAIT;
          thisTCB->task_entrance = tskBody;
          stack_init(&(thisTCB->stkTop),tskBody);
          tskStart(thisTCB);
          return thisTCB->TSK_ID;
     }
}

//以takIndex为关键字，在进程池中寻找并销毁takIndex对应的进程（需要填写）
void destroyTsk(int takIndex) {//在进程中寻找TSK_ID为takIndex的进程，并销毁该进程
	tcbPool[takIndex].task_entrance = tskEmpty;
     tcbPool[takIndex].TSK_State = TSK_NONE;
     tcbPool[takIndex].nextTCB = firstFreeTsk;
     tcbPool[takIndex].stkTop = tcbPool[takIndex].stack+STACK_SIZE-1;
     firstFreeTsk = &tcbPool[takIndex];
}

unsigned long **prevTSK_StackPtr;
unsigned long *nextTSK_StackPtr;

//切换上下文（无需填写）
void context_switch(myTCB *prevTsk, myTCB *nextTsk) {
     prevTSK_StackPtr = &(prevTsk->stkTop);
     currentTsk = nextTsk;
     nextTSK_StackPtr = nextTsk->stkTop;
     CTX_SW(prevTSK_StackPtr,nextTSK_StackPtr);
}

//FCFS调度算法（无需填写）
void scheduleFCFS(void) {
     myTCB *nextTsk;
     nextTsk = nextFCFSTsk();
     context_switch(currentTsk,nextTsk);
}

//调度算法（无需填写）
void schedule(void) {
     scheduleFCFS();
}

//进入多任务调度模式(无需填写)
unsigned long BspContextBase[STACK_SIZE];
unsigned long *BspContext;
void startMultitask(void) {
     BspContext = BspContextBase + STACK_SIZE -1;
     prevTSK_StackPtr = &BspContext;
     currentTsk = nextFCFSTsk();
     nextTSK_StackPtr = currentTsk->stkTop;
     CTX_SW(prevTSK_StackPtr,nextTSK_StackPtr);
}

//准备进入多任务调度模式(无需填写)
void TaskManagerInit(void) {
     // 初始化进程池（所有的进程状态都是TSK_NONE）
     int i;
     myTCB * thisTCB;
     for(i=0;i<TASK_NUM;i++){//对进程池tcbPool中的进程进行初始化处理
          thisTCB = &tcbPool[i];
          thisTCB->TSK_ID = i;
          thisTCB->stkTop = thisTCB->stack+STACK_SIZE-1;//将栈顶指针复位
          thisTCB->TSK_State = TSK_NONE;//表示该进程池未分配，可用
          thisTCB->task_entrance = tskEmpty;
          if(i==TASK_NUM-1){
               thisTCB->nextTCB = (void *)0;
          }
          else{
               thisTCB->nextTCB = &tcbPool[i+1];
          }
     }
     //创建idle任务
     idleTsk = &tcbPool[0];
     stack_init(&(idleTsk->stkTop),tskIdleBdy);
     idleTsk->task_entrance = tskIdleBdy;
     idleTsk->nextTCB = (void *)0;
     idleTsk->TSK_State = TSK_RDY;
     rqFCFSInit(idleTsk);

     firstFreeTsk = &tcbPool[1];
     
     //创建init任务
     createTsk(initTskBody);
    
     //进入多任务状态
     myPrintk(0x2,"START MULTITASKING......\n");
     startMultitask();
     myPrintk(0x2,"STOP MULTITASKING......SHUT DOWN\n");

}
