/*
 * debugTask.c
 *
 *  Created on: Dec 11, 2019
 *      Author: TBiberdorf
 */

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "MK60D10.h"
#include "fsl_uart.h"
#include "CLI/Cli.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* UART instance and clock */
#define DEBUG_UART UART3
#define DEBUG_UART_CLKSRC kCLOCK_BusClk
#define DEBUG_UART_CLK_FREQ CLOCK_GetFreq(kCLOCK_BusClk)
#define DEBUG_UART_IRQn UART3_RX_TX_IRQn
#define DEBUG_UART_IRQHANDLER UART3_RX_TX_IRQHandler
#define BOARD_DEBUG_UART_BAUDRATE 115200

/*! @brief Ring buffer size (Unit: Byte). */
#define DEBUG_RING_BUFFER_SIZE 80

/*! @brief Ring buffer to save received data. */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

uint8_t g_tipString[] =
    "Uart functional API interrupt example\r\nBoard receives characters then sends them out\r\nNow please input:\r\n";

/*
  Ring buffer for data input and output, in this example, input data are saved
  to ring buffer in IRQ handler. The main function polls the ring buffer status,
  if there are new data, then send them out.
  Ring buffer full: (((rxIndex + 1) % DEMO_RING_BUFFER_SIZE) == txIndex)
  Ring buffer empty: (rxIndex == txIndex)
*/
uint8_t tl_DebugInRingBuffer[DEBUG_RING_BUFFER_SIZE];
volatile uint16_t tl_DebugInRingEndIdx; /* Index of the data to send out. */
volatile uint16_t tl_DebugInRingStartIdx; /* Index of the memory to save new arrived data. */

uint8_t tl_DebugOutRingBuffer[DEBUG_RING_BUFFER_SIZE];
volatile uint16_t tl_DebugOutRingEndIdx; /* Index of the data to send out. */
volatile uint16_t tl_DebugOutRingStartIdx; /* Index of the memory to save new arrived data. */

/*******************************************************************************
 * Code
 ******************************************************************************/

void DEBUG_UART_IRQHANDLER(void)
{
    uint8_t data;
    /* If new data arrived. */
    if ((kUART_RxDataRegFullFlag | kUART_RxOverrunFlag) & UART_GetStatusFlags(DEBUG_UART))
    {
        data = UART_ReadByte(DEBUG_UART);

        /* If ring buffer is not full, add data to ring buffer. */
        if (((tl_DebugInRingStartIdx + 1) % DEBUG_RING_BUFFER_SIZE) != tl_DebugInRingEndIdx)
        {
            tl_DebugInRingBuffer[tl_DebugInRingStartIdx] = data;
            tl_DebugInRingStartIdx++;
            tl_DebugInRingStartIdx %= DEBUG_RING_BUFFER_SIZE;
        }
    }
}

/**
 * @brief output write data to debug uart port
 *
 * @param WriteData pointer to buffer to write
 * @param Size number of bytes to send
 * @return retCode
 */
uint32_t DebugTaskWrite(const uint8_t *WriteData,size_t Size)
{
	uint32_t retCode = 0;
	uint16_t idx;
	for(idx=0; idx < Size; idx++)
	{
        if (((tl_DebugOutRingStartIdx + 1) % DEBUG_RING_BUFFER_SIZE) != tl_DebugOutRingEndIdx)
        {
            tl_DebugOutRingBuffer[tl_DebugOutRingStartIdx] = WriteData[idx];
            tl_DebugOutRingStartIdx++;
            tl_DebugOutRingStartIdx %= DEBUG_RING_BUFFER_SIZE;
        }
        else
        {
        	retCode = idx;
        	break;
        }
	}

	return retCode;
}

void DebugTask(void *pvParameters)
{
    uart_config_t config;
    /*
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kUART_ParityDisabled;
     * config.stopBitCount = kUART_OneStopBit;
     * config.txFifoWatermark = 0;
     * config.rxFifoWatermark = 1;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    UART_GetDefaultConfig(&config);
    config.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;
    config.enableTx = true;
    config.enableRx = true;

    UART_Init(DEBUG_UART, &config, DEBUG_UART_CLK_FREQ);

    /* Send g_tipString out. */
    UART_WriteBlocking(DEBUG_UART, g_tipString, sizeof(g_tipString) / sizeof(g_tipString[0]));

    /* Enable RX interrupt. */
    UART_EnableInterrupts(DEBUG_UART, kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable);
    NVIC_SetPriority(DEBUG_UART_IRQn, 1);
    EnableIRQ(DEBUG_UART_IRQn);

	printf("CLI/Debug Task started\r\n");

	while(1)
	{

        /* Send data only when UART TX register is empty and ring buffer has data to send out. */
        while (tl_DebugInRingStartIdx != tl_DebugInRingEndIdx)
        {
        	// determine if CLI input command is present
            cli(tl_DebugInRingBuffer[tl_DebugInRingEndIdx]);
            tl_DebugInRingEndIdx++;
            tl_DebugInRingEndIdx %= DEBUG_RING_BUFFER_SIZE;
        }
        while ((kUART_TxDataRegEmptyFlag & UART_GetStatusFlags(DEBUG_UART)) && (tl_DebugOutRingStartIdx != tl_DebugOutRingEndIdx))
        {
        	// output information to debug port
            UART_WriteByte(DEBUG_UART, tl_DebugOutRingBuffer[tl_DebugOutRingEndIdx]);
            tl_DebugOutRingEndIdx++;
            tl_DebugOutRingEndIdx %= DEBUG_RING_BUFFER_SIZE;
        }

		vTaskDelay(10);
	}
}
