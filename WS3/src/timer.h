/*
 * timer.h
 *
 *  Created on: 17 янв. 2017 г.
 *      Author: artyom
 */

#include "stm32f4xx.h"

#ifndef TIMER_H_
#define TIMER_H_

#define DEFAULT_TIM_PERIOD 	(5)

TIM_HandleTypeDef TIM2ini;

void ConfigureTIM2(void);

#endif /* TIMER_H_ */
