#include "../include/task.h"

int base_createTsk(void (*tskBody)(void), int priority);

int FCFS_createTsk(void (*tskBody)(void), tskPara para) {
    return base_createTsk(tskBody, para.arrTime);
}
