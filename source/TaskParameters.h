/*
 * TaskParameters.h
 *
 *  Created on: Dec 11, 2019
 *      Author: TBiberdorf
 */

#ifndef TASKPARAMETERS_H_
#define TASKPARAMETERS_H_

// Debug Task
#define DEBUG_TASK_PRIORITY (configMAX_PRIORITIES - 1)
#define DEBUG_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE + 10)


// GPIO Task
#define GPIO_TASK_PRIORITY (configMAX_PRIORITIES - 1)
#define GPIO_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE + 10)


// LED Task
#define LED_TASK_PRIORITY (configMAX_PRIORITIES - 1)
#define LED_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE + 10)


#endif /* TASKPARAMETERS_H_ */
