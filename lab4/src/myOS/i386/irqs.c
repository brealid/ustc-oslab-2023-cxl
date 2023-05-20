#include "../include/vga.h"

void ignoreIntBody(void){
    append2specificPosition("Unknown interrupt", 0x07, 24, 0);
}
