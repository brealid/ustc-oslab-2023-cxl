/*
 * 本文件实现vga的相关功能，清屏和屏幕输出
 * clear_screen和append2screen必须按照如下借口实现
 * 可以增加其他函数供clear_screen和append2screen调用
 */
#define VGA_BASE (char *)0xB8000	// VGA初始地址
#define VGA_WIDTH 80				// VGA屏幕每行的宽度
#define VGA_HEIGHT 25				// VGA屏幕每列的高度
#define CURSOR_PORT_INDEX 0x3D4		// 光标-索引串口
#define CURSOR_PORT_DATA 0x3D5		// 光标-数据串口

typedef unsigned char uint8;
typedef unsigned short uint16;

extern void outb(uint16 port_to, uint8 value);
extern uint8 inb(uint16 port_from);

// 调试用
extern void uart_put_char(uint8 c);
extern void uart_put_chars(char *str);

//VGA字符界面规格：25行80列
//VGA显存初始地址为0xB8000

short cur_line = 0, cur_column = 0;		//当前光标位置
char *vga_screen = VGA_BASE;

// 通过当前行值 cur_line 与列值 cur_column 回写光标
void update_cursor(void){
	uint16 pos = cur_line * VGA_WIDTH + cur_column;
  	outb(CURSOR_PORT_INDEX, 0x0E);
	outb(CURSOR_PORT_DATA, pos >> 8);	// 写高 8bit
	outb(CURSOR_PORT_INDEX, 0x0F);
	outb(CURSOR_PORT_DATA, pos & 0xff);	// 写低 8bit
}

// 获得当前光标，计算出 cur_line 和 cur_column 的值
void get_cursor_position(void){
	uint16 pos = 0;
  	outb(CURSOR_PORT_INDEX, 0x0E);
	pos |= inb(CURSOR_PORT_DATA) << 8;	// 读高 8bit
	outb(CURSOR_PORT_INDEX, 0x0F);
	pos |= inb(CURSOR_PORT_DATA);		// 读低 8bit
	cur_line = pos / 80;
	cur_column = pos % 80;
}

// 清屏
void clear_screen(void) {
	for (int i = 0; i < VGA_HEIGHT; ++i)
		for (int j = 0; j < VGA_WIDTH * 2; j += 2) {
			vga_screen[i * VGA_WIDTH * 2 + j] = 0x00;
			vga_screen[i * VGA_WIDTH * 2 + j + 1] = 0x07;
		}
	cur_column = cur_line = 0;
	update_cursor();
}

// 向上滚屏一行
void scroll_up() {
	uart_put_chars("[+] scroll_up called\n");
	for (int i = 1; i < VGA_HEIGHT; ++i)
		for (int j = 0; j < VGA_WIDTH * 2; ++j)
			vga_screen[(i - 1) * VGA_WIDTH * 2 + j] = vga_screen[i * VGA_WIDTH * 2 + j];
	for (int i = 0; i < VGA_WIDTH * 2; i += 2) {
		vga_screen[(VGA_HEIGHT - 1) * VGA_WIDTH * 2 + i] = 0x00;
		vga_screen[(VGA_HEIGHT - 1) * VGA_WIDTH * 2 + i + 1] = 0x07;
	}
    cur_line = 24;
}

// 输出字符及其颜色
void color_putchar(uint8 ch, uint8 color) {
	get_cursor_position();
    switch (ch) {
        case '\r':
        case '\n':
			cur_column = 80;
			break;
        case '\t':
			cur_column = ((cur_column >> 2) + 1) << 2;
			break;
		default:
			vga_screen[(cur_line * VGA_WIDTH + cur_column) * 2] = ch;
			vga_screen[(cur_line * VGA_WIDTH + cur_column) * 2 + 1] = color;
			++cur_column;
            break;
    }
	if (cur_column == VGA_WIDTH) {
		cur_column = 0;
		++cur_line;
	}
	if (cur_line == VGA_HEIGHT) scroll_up();
    update_cursor();
	// 以下：串口输出调试信息
	uart_put_chars("[+] putchar: {");
	switch (ch) {
		case '\r': uart_put_char('\\'); uart_put_char('r'); break;
		case '\n': uart_put_char('\\'); uart_put_char('n'); break;
		case '\t': uart_put_char('\\'); uart_put_char('t'); break;
		default: uart_put_char(ch); break;
	}
	uart_put_chars("} ==> x=");
	uart_put_char('0' | (cur_line / 10));
	uart_put_char('0' | (cur_line % 10));
	uart_put_chars(", y=");
	uart_put_char('0' | (cur_column / 10));
	uart_put_char('0' | (cur_column % 10));
	uart_put_chars("\n");
}

void append2screen(const char *str, int color) { 
	while (*str) color_putchar(*str++, color);
}