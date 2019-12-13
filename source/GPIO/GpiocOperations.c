/*
 * GpiocOperations.c
 *
 *  Created on: Dec 13, 2019
 *      Author: TBiberdorf
 */

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "MK60D10.h"
#include "fsl_uart.h"

#include "fsl_debug_console.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_common.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define BOARD_INPUT_IRQ_HANDLER GPIO_1_IRQHANDLER

#define BOARD_TestC1_GPIO GPIOC
#define BOARD_TestC1_PORT PORTC
#define BOARD_TestC1_PIN 1
#define BOARD_SW_IRQ BOARD_SW1_IRQ
#define BOARD_SW_NAME BOARD_SW1_NAME

static uint32_t tl_ButtonPressed = false;

typedef enum _PullUpPower_e
{
	ePullUpPowerLow,
	ePullUpPowerHigh,
}ePullUpPower_t;

typedef enum _PassiveFilter_e
{
	ePassiveFilterOff,
	ePassiveFilterOn,
}ePassiveFilter_t;

typedef enum _ActiveFilter_e
{
	eActiveFilterOff,
	eActiveFilterOn,
}eActiveFilter_t;

typedef struct _InputPinOperation_s
{
	uint32_t PinSelection;
	ePullUpPower_t PullUpPower;
	ePassiveFilter_t PassiveFilter;
	ePassiveFilter_t ActiveFilter;
}sInputPinOperation_t;

sInputPinOperation_t tl_CurrentInputPin = {
		.PullUpPower=ePullUpPowerHigh,
		.PassiveFilter=ePassiveFilterOn,
		.ActiveFilter=eActiveFilterOff};

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Interrupt service fuction of switch.
 *
 * This function toggles the LED
 */
void PORTC_IRQHandler(void)
{
	uint32_t irqPinState;

	irqPinState = GPIO_GetPinsInterruptFlags(BOARD_TestC1_GPIO);

	if( irqPinState & (1U << BOARD_TestC1_PIN))
	{
    /* Clear external interrupt flag. */
		GPIO_ClearPinsInterruptFlags(BOARD_TestC1_GPIO, 1U << BOARD_TestC1_PIN);
	}
    /* Change state of button. */
    tl_ButtonPressed = true;
}


/**
 * @brief set the input pin
 */
uint32_t setInputPin(uint32_t Pin)
{

	return Pin;
}

void getInputPinConfiguration(char *CurrentConfigurationPtr,size_t *Size)
{
	size_t sentSize = 0;
	char *ptrDetails = CurrentConfigurationPtr;
	sentSize = strlen("Input Pin: ");
	memcpy(ptrDetails,"Input Pin: ",sentSize);
	*Size = sentSize;
}

void GpioTask(void *pvParameters)
{
	printf("Gpio Task started\r\n");

	while(1)
	{
		vTaskDelay(1000);
	}
}
