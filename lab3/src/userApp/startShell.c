#include "io.h"
#include "myPrintk.h"
#include "uart.h"
#include "vga.h"
#include "i8253.h"
#include "i8259A.h"
#include "tick.h"
#include "wallClock.h"

#define PART_LENGTH 64

int atoi(const unsigned char *src){
    int flags = 0;
    if (*src == '-') {
        flags = 1;
        src++;
    }
    int n = 0;
    while (*src) {
        if (*src < '0' || *src > '9') break;
        n = n * 10 + (*src++ & 15);
    }
    if (flags) n = -n;
    return n;
}

int strncpy(const unsigned char *src, unsigned char *dst, unsigned int n){
    int nn=0;
    while(*src) {
        *dst++ = *src++;
        nn++;
	    if (nn >= n) break;
    }
    return nn;
}

int strcmp(const unsigned char *str1, const unsigned char *str2){
    unsigned char c1, c2;
    do {
        c1 = *str1++;
        c2 = *str2++;
        if (c1 != c2) return (c1 > c2)?1:-1;
        if (c1 == '\0') break;
    } while(1);
    return 0;  //equal    
}

typedef int commandFunction(int argc, char (*argv)[PART_LENGTH]);

typedef struct myCommand {
    char name[PART_LENGTH];
    char help_content[200];
    commandFunction *func;
} myCommand;

myCommand cmdList[200];
int cmdPtr = 0;

void registerCommand(const char *name, const char *help_content, commandFunction *func) {
    strncpy(name, cmdList[cmdPtr].name, PART_LENGTH);
    strncpy(help_content, cmdList[cmdPtr].help_content, 200);
    cmdList[cmdPtr].func = func;
    ++cmdPtr;
}

int func_cmd(int argc, char (*argv)[PART_LENGTH]) {
    myPrintk(0x07, "oslab3 bash, These shell commands are defined internally.\nType `cmd' to see this list.\n\n");
    for (int i = 0; i < cmdPtr; ++i)
        myPrintk(0x07, "%s\n", cmdList[i].name);
    return 0;
}

int func_help(int argc, char (*argv)[PART_LENGTH]) {
    if (argc == 1) {
        myPrintk(0x07, "help: no argument.\n");
        return 1;
    }
    for (int i = 0; i < cmdPtr; ++i)
        if (!strcmp(argv[1], cmdList[i].name)) {
            myPrintk(0x07, "%s\n", cmdList[i].help_content);
            return 0;
        }
    myPrintk(0x07, "help: no help topics match '%s'.\n", argv[1]);
    return 1;
}

int func_whoami(int argc, char (*argv)[PART_LENGTH]) {
    myPrintk(0x07, "brealid\n");
    return 1;
}

int func_div0(int argc, char (*argv)[PART_LENGTH]) {
    myPrintk(0x07, "test: div0 interrupt\n");
    int a = 3 / 0;
    return 1;
}

int func_echo(int argc, char (*argv)[PART_LENGTH]) {
    for (int i = 1; i < argc; ++i)
        myPrintk(0x07, "%s%c", argv[i], " \n"[i == argc - 1]);
    return 1;
}

int func_time(int argc, char (*argv)[PART_LENGTH]) {
    if (argc == 1) {
        int h, m, s;
        getWallClock(&h, &m, &s);
        myPrintk(0x07, "Current time: %02d:%02d:%02d\n", h, m, s);
        return 0;
    } else if (argc == 4) {
        int h = atoi(argv[1]), m = atoi(argv[2]), s = atoi(argv[3]);
        setWallClock(h, m, s);
        getWallClock(&h, &m, &s);
        myPrintk(0x07, "New time has been set to: %02d:%02d:%02d\n", h, m, s);
        return 0;
    }
    myPrintk(0x07, "Usage: time\n   or: time [h] [m] [s]\n");
    return 1;
}

void startShell(void)
{
    registerCommand("cmd", "Usage: cmd\n\nList all command\n", func_cmd);
    registerCommand("help", "Usage: help [command]\n\nDisplay info about [command]\n", func_help);
    registerCommand("whoami", "Usage: whoami\n\nPrint the user name associated with the current effective user ID.\n", func_whoami);
    registerCommand("div0", "Usage: div0\n\n(Test only) Throw a divide0 interrupt\n", func_div0);
    registerCommand("echo", "Usage: echo [arg ...]\n\nWrite arguments to the standard output.\n", func_echo);
    registerCommand("time", "Usage: time\n   or: time [h] [m] [s]\n\nQuery current time.\nOr change current time according to command arguments\n", func_time);
    // 我们通过串口来实现数据的输入
    char BUF[1024];   // 输入缓存区
    int BUF_len = 0; // 输入缓存区的长度

    int argc;
    char argv[8][PART_LENGTH];

    do
    {
        BUF_len = 0;
        myPrintk(0x02, "brealid@oslab3");
        myPrintk(0x07, ":");
        myPrintk(0x09, "~");
        myPrintk(0x07, "$ ");
        while ((BUF[BUF_len] = uart_get_char()) != '\r')
            uart_put_char(BUF[BUF_len++]); // 将串口输入的数存入BUF数组中
        uart_put_chars(" -pseudo_terminal\n");
        BUF[BUF_len] = '\0';
        myPrintk(0x07, "%s\n", BUF);

        argc = 0;
        int p = 0;
        for (int i = 0; i < BUF_len; ++i)
            if (BUF[i] == ' ') {
                if (p) {
                    argv[argc++][p] = '\0';
                    p = 0;
                }
            } else {
                argv[argc][p++] = BUF[i];
            }
        argv[argc][p] = '\0';
        if (p) ++argc;
        if (argc == 0) continue;
        int success = 0;
        for (int i = 0; i < cmdPtr; ++i)
            if (!strcmp(argv[0], cmdList[i].name)) {
                cmdList[i].func(argc, argv);
                success = 1;
            }
        if (!success) myPrintk(0x07, "command '%s' not found\n", argv[0]);

    } while (1);
}
