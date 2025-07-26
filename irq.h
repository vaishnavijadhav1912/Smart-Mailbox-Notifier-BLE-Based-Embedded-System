/*
 * irq.h
 *
 *  Created on: Jan 30, 2025
 *      Author: Vaishnavi Jadhav
 */

#ifndef IRQ_H_
#define IRQ_H_

#include <stdint.h>

void powerManagerEnable(void);
void powerManagerDisable(void);
void LETIMER0_IRQHandler(void);
uint32_t letimerMilliseconds(void);
void I2C0_IRQHandler(void);
void GPIO_EVEN_IRQHandler(void);
void GPIO_ODD_IRQHandler(void);


#endif /* IRQ_H_ */
