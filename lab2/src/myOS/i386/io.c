/*
 * IO 操作
 * 理解了outb函数后实现inb函数
 */

typedef unsigned char uint8;
typedef unsigned short uint16;

uint8 inb(uint16 port_from) {
    uint8 _value;
    __asm__ __volatile__("inb %w1,%0":"=a"(_value):"Nd"(port_from));
    return _value;
}

void outb(uint16 port_to, uint8 value) {
    __asm__ __volatile__("outb %b0,%w1"::"a"(value),"Nd"(port_to));
}