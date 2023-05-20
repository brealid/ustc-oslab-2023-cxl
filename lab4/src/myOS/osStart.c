#include "include/i8253.h"
#include "include/i8259.h"
#include "include/irq.h"
#include "include/uart.h"
#include "include/vga.h"
#include "include/mem.h"
#include "include/myPrintk.h"

extern void myMain(void);

// void pressAnyKeyToStart(void){
// 	myPrintk(0x5,"Prepare uart device\n");
// 	myPrintk(0x5,"Then, press any key to start ...\n");

// 	uart_get_char();
// }

void osStart(void){
	// pressAnyKeyToStart();
	init8259A();
	init8253();
	enable_interrupt();

	clear_screen();

	pMemInit();  //建立内存管理机制

	{
		unsigned long tmp = dPartitionAlloc(kMemHandler,100);
		dPartitionWalkByAddr(kMemHandler);
		dPartitionFree(kMemHandler,tmp);
		dPartitionWalkByAddr(kMemHandler);
	}

	// 完成内核初始化，进入用户APP
    myPrintk(0x07, "System MyOS.oslab-3 Started\n\n");
	myMain();
    myPrintk(0x07, "System Shutdown\n");
	while(1);
}
