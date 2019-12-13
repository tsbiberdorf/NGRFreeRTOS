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
static uint32_t tl_ChangeConfigurationFlag = 0;

typedef enum _PinSelection_e
{
	ePinSelectedNone,
	ePinSelectedOne,
}ePinSelection_t;

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
	ePinSelection_t PinSelection;
	ePullUpPower_t PullUpPower;
	ePassiveFilter_t PassiveFilter;
	ePassiveFilter_t ActiveFilter;
}sInputPinOperation_t;

sInputPinOperation_t tl_CurrentInputPin = {
		.PinSelection = ePinSelectedOne,
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

	tl_CurrentInputPin.PinSelection = ePinSelectedOne;
	tl_ChangeConfigurationFlag = 1;
	return tl_CurrentInputPin.PinSelection = ePinSelectedOne;
}

/**
 * @brief modify the Pullup power selection
 */
uint32_t setPullupPowerLevel(uint32_t PowerLevel)
{
	uint32_t powerLevel = 0;
	if(PowerLevel)
	{
		tl_CurrentInputPin.PullUpPower = ePullUpPowerHigh;
		powerLevel = 1;
	}
	else
	{
		tl_CurrentInputPin.PullUpPower = ePullUpPowerLow;
	}

	tl_ChangeConfigurationFlag = 1;
	return powerLevel;
}

/**
 * @brief modify the passive filter selection
 */
uint32_t setPassiveFilter(uint32_t PassiveFilter)
{
	uint32_t passiveFilter= 0;
	if(PassiveFilter)
	{
		tl_CurrentInputPin.PassiveFilter = ePassiveFilterOn;
		passiveFilter = 1;
	}
	else
	{
		tl_CurrentInputPin.PassiveFilter = ePassiveFilterOff;
	}

	tl_ChangeConfigurationFlag = 1;
	return passiveFilter;
}

/**
 * @brief modify the active filter selection
 */
uint32_t setActiveFilter(uint32_t ActiveFilter)
{
	uint32_t activeFilter= 0;
	if(ActiveFilter)
	{
		tl_CurrentInputPin.ActiveFilter = eActiveFilterOn;
		ActiveFilter = 1;
	}
	else
	{
		tl_CurrentInputPin.ActiveFilter = eActiveFilterOff;
	}

	tl_ChangeConfigurationFlag = 1;
	return ActiveFilter;
}

/**
 * @brief display current input pin selection settings
 */
void getInputPinConfiguration(char *CurrentConfigurationPtr,size_t *Size)
{
	const char *pinConfigStr1 ="Input Pin: ";
	const char *pinConfigStr2 ="one\r\n";
	const char *pinConfigStr3 ="none\r\n";
	const char *pinConfigStr4 ="Pullup Pwr: ";
	const char *pinConfigStr5 ="high\r\n";
	const char *pinConfigStr6 ="low\r\n";
	const char *pinConfigStr7 ="Passive Filter: ";
	const char *pinConfigStr8 ="on\r\n";
	const char *pinConfigStr9 ="off\r\n";
	const char *pinConfigStr10 ="Active Filter: ";
	const char *pinConfigStr11 ="on\r\n";
	const char *pinConfigStr12 ="off\r\n";

	size_t sentSize = 0;
	size_t length;
	char *ptrDetails = CurrentConfigurationPtr;
	length = strlen(pinConfigStr1);
	memcpy(ptrDetails,pinConfigStr1,length);
	ptrDetails += length;
	sentSize += length;
	switch (tl_CurrentInputPin.PinSelection)
	{
	case ePinSelectedOne:
		length = strlen(pinConfigStr2);
		memcpy(ptrDetails,pinConfigStr2,length);
		ptrDetails += length;
		sentSize += length;
		break;
	case ePinSelectedNone:
		length = strlen(pinConfigStr3);
		memcpy(ptrDetails,pinConfigStr3,length);
		ptrDetails += length;
		sentSize += length;
		break;
	}


	// display pullup strength
	length = strlen(pinConfigStr4);
	memcpy(ptrDetails,pinConfigStr4,length);
	ptrDetails += length;
	sentSize += length;
	switch (tl_CurrentInputPin.PullUpPower)
	{
	case ePullUpPowerHigh:
		length = strlen(pinConfigStr5);
		memcpy(ptrDetails,pinConfigStr5,length);
		ptrDetails += length;
		sentSize += length;
		break;
	case ePullUpPowerLow:
		length = strlen(pinConfigStr6);
		memcpy(ptrDetails,pinConfigStr6,length);
		ptrDetails += length;
		sentSize += length;
		break;
	}

	// display passive filter settings
	length = strlen(pinConfigStr7);
	memcpy(ptrDetails,pinConfigStr7,length);
	ptrDetails += length;
	sentSize += length;
	switch (tl_CurrentInputPin.PassiveFilter)
	{
	case ePassiveFilterOn:
		length = strlen(pinConfigStr8);
		memcpy(ptrDetails,pinConfigStr8,length);
		ptrDetails += length;
		sentSize += length;
		break;
	case ePassiveFilterOff:
		length = strlen(pinConfigStr9);
		memcpy(ptrDetails,pinConfigStr9,length);
		ptrDetails += length;
		sentSize += length;
		break;
	}


	// display Active filter settings
	length = strlen(pinConfigStr10);
	memcpy(ptrDetails,pinConfigStr10,length);
	ptrDetails += length;
	sentSize += length;
	switch (tl_CurrentInputPin.ActiveFilter)
	{
	case eActiveFilterOn:
		length = strlen(pinConfigStr11);
		memcpy(ptrDetails,pinConfigStr11,length);
		ptrDetails += length;
		sentSize += length;
		break;
	case eActiveFilterOff:
		length = strlen(pinConfigStr12);
		memcpy(ptrDetails,pinConfigStr12,length);
		ptrDetails += length;
		sentSize += length;
		break;
	}

	*Size = sentSize;
}

void GpioTask(void *pvParameters)
{
	printf("Gpio Task started\r\n");

	while(1)
	{
		if(tl_ChangeConfigurationFlag)
		{
			tl_ChangeConfigurationFlag = 0;
			printf("change gpio input configuration\r\n");
		}
		vTaskDelay(1000);
	}
}
