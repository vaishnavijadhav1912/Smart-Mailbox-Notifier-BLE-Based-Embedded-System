/*
 * letimer.h
 *
 *  Created on: Jan 30, 2025
 *      Author: Vaishnavi Jadhav
 */

#ifndef LETIMER_H_
#define LETIMER_H_

#include "em_letimer.h"


void letimer_init(void);

void timerWaitUs(uint32_t us);

void timerWaitUs_irq(uint32_t us_wait);

#endif /* LETIMER_H_ */
