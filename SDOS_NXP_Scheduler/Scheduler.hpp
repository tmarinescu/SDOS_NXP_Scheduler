#ifndef _KERNEL_H_
#define _KERNEL_H_

#include "board.h"

extern volatile uint32_t tickCounter;

extern "C"
{
	void SDOS_Tick(void);
	void SDOS_Setup(void);
	void SDOS_Scheduler(void);
	void SDOS_Start(void);
}

void SetStack(uint32_t thread, uint32_t stack);
void AddTask(void(*task)(void));

class Task
{
public:
	void(*volatile Function)(void);
	Task();
	~Task();
};

class Thread
{
public:
	volatile uint32_t* Stack;
	volatile Task* AttachedTask;
	Thread();
	~Thread();
};

#endif
