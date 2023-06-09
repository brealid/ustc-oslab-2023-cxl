#include "io.h"

void init8253(void) {
    outb(0x43, 0x34);
    // i8253 的时钟频率为 1.193182MHz
	// 分频参数可以通过以下公式计算：分频参数 = (时钟频率/中断频率)-1
	// 将100HZ代入公式，得到：分频参数 = 11931
    outb(0x40, 11931 & 0xff);
    outb(0x40, 11931 >> 8);
    outb(0x21, inb(0x21) & 0xFE);
}