/*
 * TaskParameters.h
 *
 *  Created on: Dec 11, 2019
 *      Author: TBiberdorf
 */

#ifndef TASKPARAMETERS_H_
#define TASKPARAMETERS_H_

// Debug Task
void DebugTask(void *pvParameters);
#define DEBUG_TASK_PRIORITY (configMAX_PRIORITIES - 1)
#define DEBUG_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE + 10)
const char *DebugTaskName = "DebugTask";


// GPIO Task
void GpioTask(void *pvParameters);
#define GPIO_TASK_PRIORITY (configMAX_PRIORITIES - 1)
#define GPIO_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE + 10)
const char *GpioTaskName = "GpioTask";


#endif /* TASKPARAMETERS_H_ */
