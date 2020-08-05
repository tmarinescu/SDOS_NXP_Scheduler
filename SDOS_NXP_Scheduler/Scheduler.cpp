#include "Scheduler.hpp"

extern "C"
{
	volatile uint32_t **SDOS_StackPtr;
	uint32_t StackRAM[64 * 4];
}

volatile uint32_t tickCounter = 0;

extern "C" void SDOS_Tick(void)
{
	tickCounter++;
}

Thread::Thread()
{
	AttachedTask = 0;
}

Thread::~Thread()
{
	
}

Task::Task()
{
	Function = 0;
}

Task::~Task()
{
	
}

Thread Threads[4];
Task Tasks[4];

void Thread1(void)
{
	while (true)
	{
		if (Threads[0].AttachedTask != 0)
		{
			(*Threads[0].AttachedTask->Function)();
		}
	}
}

void Thread2(void)
{
	while (true)
	{
		if (Threads[1].AttachedTask != 0)
		{
			(*Threads[1].AttachedTask->Function)();
		}
	}
}

void Thread3(void)
{
	while (true)
	{
		if (Threads[2].AttachedTask != 0)
		{
			(*Threads[2].AttachedTask->Function)();
		}
	}
}

void Thread4(void)
{
	while (true)
	{
		if (Threads[3].AttachedTask != 0)
		{
			(*Threads[3].AttachedTask->Function)();
		}
	}
}

uint32_t StackOffset = 0;
void SetStack(uint32_t thread, uint32_t stack)
{
	for (uint32_t i = StackOffset; i < stack + StackOffset; i++)
	{
		StackRAM[0] = 0;
	}
	
	Threads[thread].Stack = &StackRAM[StackOffset + stack - 16];   //SP
	if(thread == 0)
		StackRAM[StackOffset + stack - 2] = (uint32_t)&Thread1;   //PC
	else if(thread == 1)
		StackRAM[StackOffset + stack - 2] = (uint32_t)&Thread2;   //PC
	else if(thread == 2)
		StackRAM[StackOffset + stack - 2] = (uint32_t)&Thread3;   //PC
	else
		StackRAM[StackOffset + stack - 2] = (uint32_t)&Thread4;   //PC
	StackRAM[StackOffset + stack - 1] = 0x01000000;   //xPSR
	StackOffset += stack;
}

uint32_t TaskOffset = 0;
void AddTask(void(*task)(void))
{
	Tasks[TaskOffset].Function = task;
	TaskOffset++;
}

uint32_t curThread = 0;
void SDOS_Scheduler(void)
{
	curThread++;
	if (curThread >= 4)
		curThread = 0;
	
	SDOS_StackPtr = &Threads[curThread].Stack;
}

void SDOS_Start(void)
{
	SDOS_StackPtr = &Threads[0].Stack;
	
	Threads[0].AttachedTask = &Tasks[0];
	Threads[1].AttachedTask = &Tasks[1];
	Threads[2].AttachedTask = &Tasks[2];
	Threads[3].AttachedTask = &Tasks[3];
	
	SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk);
	SysTick->LOAD = (uint32_t)(SystemCoreClock / 1000U - 1U);
	SysTick->VAL  = 0;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_CLKSOURCE_Msk;
	
	SDOS_Setup();
	
	for (;;)
	{
		__NOP();
	}
}