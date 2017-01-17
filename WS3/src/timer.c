/*
 * timer.c
 *
 *  Created on: 17 янв. 2017 г.
 *      Author: artyom
 */

#include "timer.h"

void ConfigureTIM2()
{
	RCC->CFGR &= ~(RCC_CFGR_SW);
	RCC->CFGR |= RCC_CFGR_SW_HSE;

	__TIM2_CLK_ENABLE();

	TIM2ini.Instance = TIM2;

	TIM2ini.Init.Prescaler = HSE_VALUE / 2000 - 1;
	TIM2ini.Init.CounterMode = TIM_COUNTERMODE_UP;
	TIM2ini.Init.Period = DEFAULT_TIM_PERIOD;

	HAL_TIM_Base_Init(&TIM2ini);
	HAL_TIM_Base_Start_IT(&TIM2ini);
}
