/*
 * GpiocOperations.h
 *
 *  Created on: Dec 13, 2019
 *      Author: TBiberdorf
 */

#ifndef GPIO_GPIOCOPERATIONS_H_
#define GPIO_GPIOCOPERATIONS_H_


void getInputPinConfiguration(char *PtrCurrentConfiguration,size_t *PtrSize);
void GpioTask(void *pvParameters);

#endif /* GPIO_GPIOCOPERATIONS_H_ */
