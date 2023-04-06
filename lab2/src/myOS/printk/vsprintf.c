/*
 * 识别格式化字符串的核心代码写在本文件中
 * 可以从网上移植代码
 */ 
 
#include <stdarg.h>  

int vsprintf(char *buf, const char *fmt, va_list argptr) {
	static char buffer[32];
	int buf_top = 0;
	char *original_buf = buf;
	while (*fmt) {
		if (*fmt == '%') {
			if (*(fmt + 1) == '%') {
				*buf++ = '%';
				fmt += 2;
				continue;
			}
			if (*(fmt + 1) == 'd') {
				int value = va_arg(argptr, int);
				if (value < 0) {
					*buf++ = '-';
					value = -value;
				}
				while (value) {
					buffer[buf_top++] = '0' | (value % 10);
					value /= 10;
				}
				while (buf_top) *buf++ = buffer[--buf_top];
				fmt += 2;
				continue;
			}
		}
		*buf++ = *fmt++;
	}
	*buf = '\0';
	return buf - original_buf;
}
