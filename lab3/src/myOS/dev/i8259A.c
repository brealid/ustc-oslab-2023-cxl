#include "io.h"

void init8259A(void){
	// 屏蔽所有中断源
    outb(0x21, 0xFF);
    outb(0xA1, 0xFF);

	// 主片初始化
    outb(0x20, 0x11); // ICW1
    outb(0x21, 0x20); // ICW2：起始向量号
    outb(0x21, 0x04); // ICW3：从片接入引脚位
    outb(0x21, 0x03); // ICW4：中断结束方式 AutoEOI

	// 从片初始化
    outb(0xA0, 0x11); // ICW1
    outb(0xA1, 0x28); // ICW2：起始向量号
    outb(0xA1, 0x02); // ICW3：接入主片的编号
    outb(0xA1, 0x01); // ICW4：中断结束方式
}