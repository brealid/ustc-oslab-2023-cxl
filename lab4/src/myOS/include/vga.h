#ifndef __VGA_H__
#define __VGA_H__

#define VGA_BASE (char *)0xB8000	// VGA初始地址
#define VGA_WIDTH 80				// VGA屏幕每行的宽度
#define VGA_HEIGHT 24				// VGA屏幕每列的高度(row25 for system)

typedef unsigned char uint8;
typedef unsigned short uint16;

void update_cursor(void);
void clear_screen(void);
void scroll_up(void);
void color_putchar(uint8 c, uint8 color);
void append2screen(const char* str, int color);
void append2specificPosition(const char* str, int color, int xx, int yy);

#endif