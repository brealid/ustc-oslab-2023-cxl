#ifndef __MEM_H__
#define __MEM_H__
unsigned long dPartitionInit(unsigned long start, unsigned long size);
unsigned long dPartitionAlloc(unsigned long dp, unsigned long size);
unsigned long dPartitionFree( unsigned long dp, unsigned long start);

void dPartitionWalkByAddr(unsigned long dp);

unsigned long eFPartitionInit(unsigned long start, unsigned long perSize, unsigned long n);
unsigned long eFPartitionTotalSize(unsigned long perSize, unsigned long n);
unsigned long eFPartitionAlloc(unsigned long EFPHandler);
unsigned long eFPartitionFree(unsigned long EFPHandler,unsigned long mbStart);

void eFPartitionWalkByAddr(unsigned long efp);

void pMemInit(void);

unsigned long uMemHandler;
unsigned long kMemHandler;
#endif
