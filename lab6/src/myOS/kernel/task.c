#include "../include/task.h"
#include "../include/myPrintk.h"

void schedule(void);
void destroyTsk(int takIndex);

// tskIdleBdy进程
void tskIdleBdy(void) {
    while(1) {
        schedule();
    }
}

// tskEmpty 进程
void tskEmpty(void) {}


//初始化就绪队列（需要填写）
void rqInit() {//对rq进行初始化处理
    readyQueue_head = readyQueue_tail = NULL;
}

//如果就绪队列为空，返回True（需要填写）
int rqIsEmpty(void) {//当head和tail均为NULL时，rq为空
    return readyQueue_head == NULL && readyQueue_tail == NULL;
}

//获取就绪队列的头结点信息，并返回（需要填写）
myTCB * rqNextTsk(void) {//获取下一个Tsk
    if(rqIsEmpty()){
        return idleTsk;
    }
    else{
        return readyQueue_head;
    }
}

//将一个未在就绪队列中的TCB加入到就绪队列中（需要填写）
void tskEnqueue(myTCB *tsk) {//将tsk入队rq
    tsk->nextTCB = NULL;
    if(rqIsEmpty()){
        readyQueue_head = readyQueue_tail = tsk;
    }
    else if (tsk->priority < readyQueue_head->priority) {
        tsk->nextTCB = readyQueue_head;
        readyQueue_head = tsk;
        context_switch(currentTsk, tsk);
    } else {
        myTCB *p = readyQueue_head;
        while (p->nextTCB != NULL && p->nextTCB->priority <= tsk->priority)
            p = p->nextTCB;
        tsk->nextTCB = p->nextTCB;
        p->nextTCB = tsk;
        if (tsk->nextTCB == NULL)
            readyQueue_tail = tsk;
    }
}

//将就绪队列中的TCB移除（需要填写）
void tskDequeue(myTCB *tsk) {//rq出队
    if (rqIsEmpty()) {
        return;
    } else {
        if (readyQueue_head == readyQueue_tail)
            readyQueue_head = readyQueue_tail = NULL;
        else
            readyQueue_head = readyQueue_head->nextTCB;
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
    tskEnqueue(tsk);
}

//进程池中一个在就绪队列中的TCB的结束（不需要填写）
void tskEnd(void){
    // myPrintk(0x7, "[tskEnd | context_switch] ");
    // context_switch(currentTsk, idleTsk);
    //将一个在就绪队列中的TCB移除就绪队列
    // myPrintk(0x7, "[tskEnd | tskDequeue] ");
    tskDequeue(currentTsk);
    //由于TCB结束，我们将进程池中对应的TCB也删除
    // myPrintk(0x7, "[tskEnd | destroyTsk] ");
    destroyTsk(currentTsk->TSK_ID);
    //TCB结束后，我们需要进行一次调度
    // myPrintk(0x7, "[tskEnd | schedule] ");
    schedule();
}

int initTsk(void (*tskBody)(void), int priority) {
    myTCB *thisTCB;
    if (firstFreeTsk == NULL){
        return -1;
    } else {
        thisTCB = firstFreeTsk;
        firstFreeTsk = firstFreeTsk->nextTCB;
        // myPrintk(0x7, "[+] Init Tsk: %d\n", thisTCB->TSK_ID);
        // myPrintk(0x7, "    Tsk Stack Before Init: %d\n", thisTCB->stkTop - thisTCB->stack + 1);
        thisTCB->TSK_State = TSK_WAIT;
        thisTCB->task_entrance = tskBody;
        thisTCB->priority = priority;
        thisTCB->stkTop = thisTCB->stack + STACK_SIZE - 1;
        stack_init(&(thisTCB->stkTop),tskBody);
        // myPrintk(0x7, "    Tsk Stack After Init: %d\n", thisTCB->stkTop - thisTCB->stack + 1);
        return thisTCB->TSK_ID;
    }
}

//以tskBody为参数在进程池中创建一个进程，并调用tskStart函数，将其加入就绪队列（需要填写）
int base_createTsk(void (*tskBody)(void), int priority){//在进程池中创建一个进程，并把该进程加入到rq队列中
    myTCB * thisTCB;
    int TSK_ID = initTsk(tskBody, priority);
    if(TSK_ID == -1){
        return -1;
    } else {
        thisTCB = &tcbPool[TSK_ID];
        tskStart(thisTCB);
        return TSK_ID;
    }
}

//以takIndex为关键字，在进程池中寻找并销毁takIndex对应的进程（需要填写）
void destroyTsk(int takIndex) {//在进程中寻找TSK_ID为takIndex的进程，并销毁该进程
    // myPrintk(0x7, "[+] Destroy Tsk: %d\n", takIndex);
	tcbPool[takIndex].task_entrance = tskEmpty;
    tcbPool[takIndex].TSK_State = TSK_NONE;
    tcbPool[takIndex].nextTCB = firstFreeTsk;
    // tcbPool[takIndex].stkTop = tcbPool[takIndex].stack+STACK_SIZE-1;
    // myPrintk(0x7, "    Stack Size reset to: %d\n", tcbPool[takIndex].stkTop - tcbPool[takIndex].stack + 1);
    firstFreeTsk = &tcbPool[takIndex];
}

unsigned long **prevTSK_StackPtr;
unsigned long *nextTSK_StackPtr;

//切换上下文（无需填写）
void context_switch(myTCB *prevTsk, myTCB *nextTsk) {
    // myPrintk(0x7, "[+] Context Switch: %d -> %d\n", prevTsk->TSK_ID, nextTsk->TSK_ID);
    // myPrintk(0x7, "    %d stack size: %d\n", prevTsk->TSK_ID, prevTsk->stkTop - prevTsk->stack + 1);
    // myPrintk(0x7, "    %d stack size: %d\n", nextTsk->TSK_ID, nextTsk->stkTop - nextTsk->stack + 1);

    prevTSK_StackPtr = &(prevTsk->stkTop);
    currentTsk = nextTsk;
    nextTSK_StackPtr = nextTsk->stkTop;
    CTX_SW();
    // myPrintk(0x7, "    %d stack size: %d\n", prevTsk->TSK_ID, prevTsk->stkTop - prevTsk->stack + 1);
    // myPrintk(0x7, "    %d stack size: %d\n", nextTsk->TSK_ID, nextTsk->stkTop - nextTsk->stack + 1);
}

//调度算法（无需填写）
void schedule(void) {
    myTCB *nextTsk;
    nextTsk = rqNextTsk();
    context_switch(currentTsk,nextTsk);
}

//进入多任务调度模式(无需填写)
unsigned long BspContextBase[STACK_SIZE];
unsigned long *BspContext;
void startMultitask(void) {
    BspContext = BspContextBase + STACK_SIZE - 1;
    prevTSK_StackPtr = &BspContext;
    currentTsk = rqNextTsk();
    nextTSK_StackPtr = currentTsk->stkTop;
    CTX_SW();
}

void printTskStatus() {
    // print Idle Tsk
    myPrintk(0x07, "[+] Idle Tsk: (%p) %d\n", idleTsk, idleTsk->TSK_ID);
    myPrintk(0x07, "              Stk Remain Size: %d\n", idleTsk->stkTop - idleTsk->stack + 1);
    // print current Tsk
    myPrintk(0x07, "[+] Current Tsk: %d\n", currentTsk->TSK_ID);
    myPrintk(0x07, "    Stk Remain Size: %d\n", currentTsk->stkTop - currentTsk->stack + 1);
    // print ready queue
    myPrintk(0x07, "[+] Ready Queue: ");
    myTCB *p = readyQueue_head;
    while (p != NULL) {
        myPrintk(0x07, "%d ", p->TSK_ID);
        p = p->nextTCB;
    }
    myPrintk(0x07, "\n");
    // print free TCB
    myPrintk(0x07, "[+] Free TCB: ");
    p = firstFreeTsk;
    while (p != NULL) {
        myPrintk(0x07, "%d ", p->TSK_ID);
        p = p->nextTCB;
    }
    myPrintk(0x07, "\n");
}

//准备进入多任务调度模式(无需填写)
void TaskManagerInit(void (*initTskBody)(void)) {
    // 初始化进程池（所有的进程状态都是TSK_NONE）
    int i;
    myTCB * thisTCB;
    for (i = 0; i < TASK_NUM; i++) { // 对进程池tcbPool中的进程进行初始化处理
        thisTCB = &tcbPool[i];
        thisTCB->TSK_ID = i;
        thisTCB->stkTop = thisTCB->stack + STACK_SIZE - 1;//将栈顶指针复位
        thisTCB->TSK_State = TSK_NONE;//表示该进程池未分配，可用
        thisTCB->task_entrance = tskEmpty;
        if (i == TASK_NUM - 1) thisTCB->nextTCB = NULL;
        else thisTCB->nextTCB = &tcbPool[i + 1];
    }
    firstFreeTsk = &tcbPool[0];//将进程池中的第一个进程的地址赋值给firstFreeTsk
    //创建idle任务
    idleTsk = &tcbPool[initTsk(tskIdleBdy, -1)];//创建idle任务

    //创建init任务
    base_createTsk(initTskBody, 0);

    //进入多任务状态
    myPrintk(0x2, "START MULTITASKING......\n");
    startMultitask();
    myPrintk(0x2, "STOP MULTITASKING......SHUT DOWN\n");
}
