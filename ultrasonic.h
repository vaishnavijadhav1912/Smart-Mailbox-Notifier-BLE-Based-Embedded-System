/*
 * ultrasonic.h
 *
 *  Created on: 28-Apr-2025
 *      Author: Anuja Joshi
 */

#ifndef SRC_ULTRASONIC_H_
#define SRC_ULTRASONIC_H_

#include <stdint.h>
void     hcsr04_init(void);
uint32_t hcsr04_read_distance_mm(void);
uint32_t ultra_task_process(void);


#endif /* SRC_ULTRASONIC_H_ */
