#include "../../include/myPrintk.h"
#include "../../include/mem.h"

#define DATA_CHECK_A 0xAA55
#define DATA_CHECK_B 0x55AA

unsigned long pMemHandler;
extern unsigned long uMemHandler;
extern unsigned long kMemHandler;

unsigned long pMemStart;  // 可用的内存的起始地址
unsigned long pMemSize;   // 可用的大小

void memTest(unsigned long start, unsigned long grainSize){
	/*功能：检测算法
		这一个函数对应实验讲解ppt中的第一大功能-内存检测。
		本函数的功能是检测从start开始有多大的内存可用，具体算法参照ppt检测算法的文字描述
	注意点三个：
	1、覆盖写入和读出就是往指针指向的位置写和读，不要想复杂。
	  (至于为什么这种检测内存的方法可行大家要自己想一下)
	2、开始的地址要大于1M，需要做一个if判断。
	3、grainsize不能太小，也要做一个if判断
	
	*/
	if (start < 0x100000) start = 0x100000;
	if (grainSize < 4) grainSize = 4;

	pMemStart = start;
	pMemSize = 0;

	unsigned long addr = start;
    unsigned short *addr_now, data_bak;

	// get the size of pMem
    while (1) {
		// check head
        addr_now = (unsigned short *)addr; 		// grain 初始 2 字节
        data_bak = *addr_now; 					// read original data
        *addr_now = DATA_CHECK_A; 				// write 0xAA55
        if (*addr_now != DATA_CHECK_A) break;
        *addr_now = DATA_CHECK_B; 				// write 0x55AA
        if (*addr_now != DATA_CHECK_B) break;
        *addr_now = data_bak; 					// write back original data

		// check head
        addr_now = (unsigned short *)(addr + grainSize - 2); // grain 末尾 2 字节
        data_bak = *addr_now; 					// read original data
        *addr_now = DATA_CHECK_A; 				// write 0xAA55
        if (*addr_now != DATA_CHECK_A) break;
        *addr_now = DATA_CHECK_B; 				// write 0x55AA
        if (*addr_now != DATA_CHECK_B) break;
        *addr_now = data_bak; 					// write back original data

		// add up
		addr += grainSize;
		pMemSize += grainSize;
    }
	myPrintk(0x7, "MemStart: %x\n", pMemStart);
	myPrintk(0x7, "MemSize:  %x\n", pMemSize);
}

extern unsigned long _end;
void pMemInit(void){
	unsigned long _end_addr = (unsigned long) &_end;
	memTest(0x100000,0x1000);
	myPrintk(0x7,"_end:  %x  \n", _end_addr);
	if (pMemStart <= _end_addr) {
		pMemSize -= _end_addr - pMemStart;
		pMemStart = _end_addr;
	}
	
	// 此处选择不同的内存管理算法
	pMemHandler = dPartitionInit(pMemStart, pMemSize);	

    // set space
    unsigned long kMemStart = dPartitionAlloc(pMemHandler, pMemSize / 3);
    unsigned long uMemStart = dPartitionAlloc(pMemHandler, pMemSize / 2);
    // init
    kMemHandler = dPartitionInit(kMemStart, pMemSize / 3);
    uMemHandler = dPartitionInit(uMemStart, pMemSize / 2);
}
