#include "../include/task.h"

int base_createTsk(void (*tskBody)(void), int priority);

int SJF_createTsk(void (*tskBody)(void), tskPara para) {
    return base_createTsk(tskBody, para.exeTime);
}
