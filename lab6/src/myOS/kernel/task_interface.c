#include "../include/task.h"

int nowScheduler = 0;

int FCFS_createTsk(void (*tskBody)(void), tskPara para);
int SJF_createTsk(void (*tskBody)(void), tskPara para);
int PRIO_createTsk(void (*tskBody)(void), tskPara para);

tskPara make_tskPara(int arrTime, int exeTime, int priority) {
    tskPara para;
    para.arrTime = arrTime;
    para.exeTime = exeTime;
    para.priority = priority;
    return para;
}

void switchScheduler(int scheduler) {
    nowScheduler = scheduler;
}

int createTsk(void (*tskBody)(void), tskPara para) {
    switch (nowScheduler) {
        case 0:
            FCFS_createTsk(tskBody, para);
            break;
        case 1:
            SJF_createTsk(tskBody, para);
            break;
        case 2:
            PRIO_createTsk(tskBody, para);
            break;
        default:
            FCFS_createTsk(tskBody, para);
            break;
    }
}
