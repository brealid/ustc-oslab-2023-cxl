#ifndef __SHELL_H__
#define __SHELL_H__

#define PART_LENGTH 64

typedef int commandFunction(int argc, char **argv);

void initShell(void);
void startShell(void);

void registerCommand(const char *name, const char *help_content, commandFunction *func);
            
#endif