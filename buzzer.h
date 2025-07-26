/*
 * buzzer.h
 *
 *  Created on: 28-Apr-2025
 *      Author: Anuja Joshi
 */

#ifndef SRC_BUZZER_H_
#define SRC_BUZZER_H_

void buzzer_gpio_init(void);
void buzzer_beep_blocking(uint16_t ms);

#endif /* SRC_BUZZER_H_ */
