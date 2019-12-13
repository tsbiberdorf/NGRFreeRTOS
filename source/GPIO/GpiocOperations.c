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



void GpioTask(void *pvParameters)
{
	printf("Gpio Task started\r\n");

	while(1)
	{
		vTaskDelay(1000);
	}
}
