/* 
 * 与 UART 相关的输出
 * 调用inb和outb函数，实现下面的uart的三个函数
 */
typedef unsigned char uint8;
typedef unsigned short uint16;

extern void outb(uint16 port_to, uint8 value);
extern uint8 inb(uint16 port_from);

#define uart_base 0x3F8

void uart_put_char(unsigned char c) {
	outb(uart_base, c);      
}

unsigned char uart_get_char(void) {
	return inb(uart_base);       
}

void uart_put_chars(char *str) { 
	while (*str) outb(uart_base, *str++);  
}