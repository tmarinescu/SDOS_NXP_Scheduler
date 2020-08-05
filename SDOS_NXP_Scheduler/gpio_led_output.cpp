/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "Scheduler.hpp"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_LED_GPIO BOARD_USER_LED_GPIO
#define EXAMPLE_LED_GPIO_PIN BOARD_USER_LED_GPIO_PIN
#define EXAMPLE_DELAY_COUNT 8000000

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief delay a while.
 */
void delay(uint32_t amnt);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* The PIN status */
volatile bool g_pinSet = false;
/*******************************************************************************
 * Code
 ******************************************************************************/
void delay(uint32_t amnt)
{
	uint32_t cur = tickCounter + amnt;
	do
	{
		__NOP();
	} while (tickCounter < cur);
}

volatile bool SetHigh = false;

void Task1()
{
	SetHigh = !SetHigh;
	delay(500);
}

void Task2()
{
	if (SetHigh == true)
		GPIO_PinWrite(EXAMPLE_LED_GPIO, EXAMPLE_LED_GPIO_PIN, 1);
	else
		GPIO_PinWrite(EXAMPLE_LED_GPIO, EXAMPLE_LED_GPIO_PIN, 0);
}

void Task3()
{
	__NOP();
}

void Task4()
{
	__NOP();
}

/*!
 * @brief Main function
 */
int main(void)
{
	/* Define the init structure for the output LED pin*/
	gpio_pin_config_t led_config = { kGPIO_DigitalOutput, 0, kGPIO_NoIntmode };
	__disable_irq();
	/* Board pin, clock, debug console init */
	BOARD_ConfigMPU();
	BOARD_InitPins();
	BOARD_BootClockRUN();
	BOARD_InitDebugConsole();

	/* Print a note to terminal. */
	PRINTF("\r\n GPIO Driver example\r\n");
	PRINTF("\r\n The LED is blinking.\r\n");

	/* Init output LED GPIO. */
	GPIO_PinInit(EXAMPLE_LED_GPIO, EXAMPLE_LED_GPIO_PIN, &led_config);
	
	SetStack(0, 64);
	SetStack(1, 64);
	SetStack(2, 64);
	SetStack(3, 64);
	AddTask(&Task1);
	AddTask(&Task2);
	AddTask(&Task3);
	AddTask(&Task4);
	
	SDOS_Start();
	
	for (;;)
	{
		__NOP();
	}
}
