#ifndef __VGA_H__
#define __VGA_H__

#define VGA_COL 80
#define VGA_ROW 25

#define VGA_BASE 0xB8000

void update_cursor(void);
void clear_screen(void);
void scroll_screen(void);
void append_char_to_screen(char c, int color);
void append2screen(char* str, int color);

#endif