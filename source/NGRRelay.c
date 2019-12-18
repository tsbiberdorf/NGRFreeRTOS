/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    NGRRelay.c
 * @brief   Application entry point.
 */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK60D10.h"
#include "fsl_debug_console.h"
#include "TaskParameters.h"
#include "UartTasks/UartTasks.h"
#include "LED/LEDOperations.h"
#include "CMSIS/RTT/SEGGER_RTT_Conf.h"
#include "CMSIS/RTT/SEGGER_RTT.h"

/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */
/*!
 * @brief Task responsible for printing of "Hello world." message.
 */

/*
 * @brief   Application entry point.
 */
int main(void) {
  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
	BOARD_InitDebugConsole();

	SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);
    SEGGER_RTT_printf(0,"Hello World\r\n");

//    xTaskCreate(DebugTask, DebugTaskName, DEBUG_TASK_STACK_SIZE , NULL, DEBUG_TASK_PRIORITY, NULL);
    StartDebugTask();
//    xTaskCreate(GpioTask, GpioTaskName, GPIO_TASK_STACK_SIZE , NULL, GPIO_TASK_PRIORITY, NULL);
    StartGpioTask();

    StartLedTask();

    vTaskStartScheduler();
    for (;;)
        ;

    return 0 ;
}
