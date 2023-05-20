#include "../../include/myPrintk.h"

#define ulong_size 4

// eFPartition是表示整个内存的数据结构
typedef struct eFPartition{
	unsigned long totalN;
	unsigned long perSize;  // unit: byte	
	unsigned long firstFree;
}eFPartition;	// 占12个字节

#define eFPartition_size 12

void showeFPartition(struct eFPartition *efp){
	myPrintk(0x5,"eFPartition(start=0x%x, totalN=0x%x, perSize=0x%x, firstFree=0x%x)\n", efp, efp->totalN, efp->perSize, efp->firstFree);
}

// 一个EEB表示一个空闲可用的Block
typedef struct EEB {
	unsigned long next_start;
}EEB;	// 占4个字节

#define EEB_size 4

void showEEB(struct EEB *eeb){
	myPrintk(0x7,"EEB(start=0x%x, next=0x%x)\n", eeb, eeb->next_start);
}


void eFPartitionWalkByAddr(unsigned long efpHandler){
	/*功能：本函数是为了方便查看和调试的。
	1. 打印eFPartiiton结构体的信息，可以调用上面的showeFPartition函数。
	2. 遍历每一个EEB，打印出他们的地址以及下一个EEB的地址（可以调用上面的函数showEEB）

	*/
	eFPartition *handler = (eFPartition *)efpHandler;
	showeFPartition(handler);

	EEB *block = (EEB *)handler->firstFree;
	while (block) {
		showEEB(block);
		block = (EEB *)block->next_start;
	}
}


unsigned long eFPartitionTotalSize(unsigned long perSize, unsigned long n){
	/*功能：计算占用空间的实际大小，并将这个结果返回
	1. 根据参数persize（每个大小）和n个数计算总大小，注意persize的对齐。
		例如persize是31字节，你想8字节对齐，那么计算大小实际代入的一个块的大小就是32字节。
	2. 同时还需要注意“隔离带”EEB的存在也会占用4字节的空间。
		typedef struct EEB {
			unsigned long next_start;
		}EEB;	
	3. 最后别忘记加上eFPartition这个数据结构的大小，因为它也占一定的空间。
	*/
	if (perSize <= 0) return 0;
	if (perSize & 3) perSize = ((perSize >> 2) + 1) << 2;
	return perSize * n + eFPartition_size;
}

unsigned long eFPartitionInit(unsigned long start, unsigned long perSize, unsigned long n){
	/*功能：初始化内存
	1. 需要创建一个eFPartition结构体，需要注意的是结构体的perSize不是直接传入的参数perSize，需要对齐。结构体的next_start也需要考虑一下其本身的大小。
	2. 就是先把首地址start开始的一部分空间作为存储eFPartition类型的空间
	3. 然后再对除去eFPartition存储空间后的剩余空间开辟若干连续的空闲内存块，将他们连起来构成一个链。注意最后一块的EEB的nextstart应该是0
	4. 需要返回一个句柄，也即返回eFPartition *类型的数据
	注意的地方：
		1.EEB类型的数据的存在本身就占用了一定的空间。
	*/
	
	if (perSize <= 0) return 0;
	if (perSize & 3) perSize = ((perSize >> 2) + 1) << 2;

	eFPartition *handler = (eFPartition *)start;
	handler->perSize = perSize;
	handler->totalN = n;
	handler->firstFree = start + sizeof(eFPartition);

	// divide this entire memory block into equal-sized blocks
	EEB *block = (EEB *)handler->firstFree;
	for (int i = 0; i < n - 1; i++) {
		block->next_start = (unsigned long)block + perSize;
		block = (EEB *)block->next_start;
	}
	block->next_start = 0;
	return start;
}


unsigned long eFPartitionAlloc(unsigned long EFPHandler){
	/*功能：分配一个空间
	1. 本函数分配一个空闲块的内存并返回相应的地址，EFPHandler表示整个内存的首地址
	2. 事实上EFPHandler就是我们的句柄，EFPHandler作为eFPartition *类型的数据，其存放了我们需要的firstFree数据信息
	3. 从空闲内存块组成的链表中拿出一块供我们来分配空间，并维护相应的空闲链表以及句柄
	注意的地方：
		1.EEB类型的数据的存在本身就占用了一定的空间。
	*/
	eFPartition *handler = (eFPartition *)EFPHandler;
	if (!handler->firstFree) return 0;

	EEB *block = (EEB *)handler->firstFree;
	handler->firstFree = block->next_start;

	return (unsigned long)block;
}


unsigned long eFPartitionFree(unsigned long EFPHandler,unsigned long mbStart){
	/*功能：释放一个空间
	1. mbstart将成为第一个空闲块，EFPHandler的firstFree属性也需要相应大的更新。
	2. 同时我们也需要更新维护空闲内存块组成的链表。
	
	*/
	/*功能：释放一个空间
	1. 按照对应的fit的算法释放空间
	2. 注意检查要释放的start~end这个范围是否在dp有效分配范围内
		返回1 没问题
		返回0 error
	3. 需要考虑两个空闲且相邻的内存块的合并
	*/
	if (mbStart < EFPHandler + eFPartition_size) return 0;

	eFPartition *handler = (eFPartition *)EFPHandler;
	if (mbStart > EFPHandler + eFPartitionTotalSize(handler->perSize, handler->totalN)) return 0;
	
	EEB *block = (EEB *)handler->firstFree, *blockPre = 0, *blockNxt = 0;

	while (block) {
		if (block > (EEB *)mbStart) {
			blockNxt = block;
			break;
		}
		blockPre = block;
		block = (EEB *)block->next_start;
	}

	block = (EEB *)mbStart;
	if (blockNxt) block->next_start = (unsigned long)blockNxt;
	else block->next_start = 0;

	if (blockPre) blockPre->next_start = mbStart;
	else handler->firstFree = mbStart;
	return 1;
	
}
