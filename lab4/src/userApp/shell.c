#include "../myOS/userInterface.h"

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

typedef int commandFunction(int argc, char **argv);

typedef struct myCommand {
    char name[PART_LENGTH];
    char help_content[200];
    commandFunction *func;
    struct myCommand *nxtCmd;
} myCommand;

myCommand *cmdList = 0;
int cmdPtr = 0;

void registerCommand(const char *name, const char *help_content, commandFunction *func) {
    myCommand *cur = (myCommand *)malloc(sizeof(myCommand));
    strncpy(name, cur->name, PART_LENGTH);
    strncpy(help_content, cur->help_content, 200);
    cur->func = func;
    cur->nxtCmd = 0;
    if (!cmdList) cmdList = cur;
    else {
        myCommand *p = cmdList;
        while (p->nxtCmd) p = p->nxtCmd;
        p->nxtCmd = cur;
    }
}

int func_cmd(int argc, char **argv) {
    myPrintf(0x07, "oslab4 bash, These shell commands are defined internally.\nType `cmd' to see this list.\n\n");
    for (myCommand *p = cmdList; p; p = p->nxtCmd)
        myPrintf(0x07, "%s\n", p->name);
    return 0;
}

int func_help(int argc, char **argv) {
    if (argc == 1) {
        myPrintf(0x07, "help: no argument.\n");
        return 1;
    }
    for (myCommand *p = cmdList; p; p = p->nxtCmd)
        if (!strcmp(argv[1], p->name)) {
            myPrintf(0x07, "%s\n", p->help_content);
            return 0;
        }
    myPrintf(0x07, "help: no help topics match '%s'.\n", argv[1]);
    return 1;
}

int func_clear(int argc, char **argv) {
    clear_screen();
    return 0;
}

int func_whoami(int argc, char **argv) {
    myPrintf(0x07, "brealid\n");
    return 1;
}

int func_echo(int argc, char **argv) {
    for (int i = 1; i < argc; ++i)
        myPrintf(0x07, "%s%c", argv[i], " \n"[i == argc - 1]);
    return 1;
}

int func_time(int argc, char **argv) {
    if (argc == 1) {
        int h, m, s;
        getWallClock(&h, &m, &s);
        myPrintf(0x07, "Current time: %02d:%02d:%02d\n", h, m, s);
        return 0;
    } else if (argc == 4) {
        int h = atoi(argv[1]), m = atoi(argv[2]), s = atoi(argv[3]);
        setWallClock(h, m, s);
        getWallClock(&h, &m, &s);
        myPrintf(0x07, "New time has been set to: %02d:%02d:%02d\n", h, m, s);
        return 0;
    }
    myPrintf(0x07, "Usage: time\n   or: time [h] [m] [s]\n");
    return 1;
}

void initShell() {
    registerCommand("cmd", "Usage: cmd\n\nList all command\n", func_cmd);
    registerCommand("help", "Usage: help [command]\n\nDisplay info about [command]\n", func_help);
    registerCommand("clear", "Usage: clear\n\nclear screen\n", func_clear);
    registerCommand("whoami", "Usage: whoami\n\nPrint the user name associated with the current effective user ID.\n", func_whoami);
    registerCommand("echo", "Usage: echo [arg ...]\n\nWrite arguments to the standard output.\n", func_echo);
    registerCommand("time", "Usage: time\n   or: time [h] [m] [s]\n\nQuery current time.\nOr change current time according to command arguments\n", func_time);
}

void startShell(void)
{
    // 我们通过串口来实现数据的输入
    char BUF[1024];   // 输入缓存区
    int BUF_len = 0; // 输入缓存区的长度

    int argc;
    char **argv = (char **)malloc(sizeof(char *) * 8);
    for (int i = 0; i < 8; ++i)
        argv[i] = (char *)malloc(sizeof(char) * PART_LENGTH);

    do {
        BUF_len = 0;
        myPrintf(0x02, "brealid@oslab4");
        myPrintf(0x07, ":");
        myPrintf(0x09, "~");
        myPrintf(0x07, "$ ");
        while ((BUF[BUF_len] = uart_get_char()) != '\r')
            uart_put_char(BUF[BUF_len++]); // 将串口输入的数存入BUF数组中
        uart_put_chars("\n");
        BUF[BUF_len] = '\0';
        myPrintf(0x07, "%s\n", BUF);

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
        for (myCommand *p = cmdList; p; p = p->nxtCmd)
            if (!strcmp(argv[0], p->name)) {
                p->func(argc, (char **)argv);
                success = 1;
                break;
            }
        if (!success) myPrintf(0x07, "command '%s' not found\n", argv[0]);

    } while (1);
}
