//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "diag/Trace.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

#define LED_NUM 				(4)
#define DEFAULT_TIM_PERIOD		(8000.0)
#define DEFAULT_TIM_PRESCALER	(0)

void SystemClockConfig(void);
void HardPWM(float);
void InitializeLED(GPIO_InitTypeDef*);

TIM_HandleTypeDef Timer;
GPIO_InitTypeDef LED[LED_NUM];

int main(int argc, char* argv[])
{
	SystemClockConfig();
	HardPWM(20);
	InitializeLED(LED);

	HAL_TIM_Base_Start(&Timer);
	HAL_TIM_PWM_Start(&Timer, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&Timer, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&Timer, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&Timer, TIM_CHANNEL_4);


	while(1);
}

void SystemClockConfig(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
}

void HardPWM(float div)
{
//	RCC->CFGR &= ~(RCC_CFGR_SW);
//	RCC->CFGR |= RCC_CFGR_SW_HSE;

	TIM_OC_InitTypeDef Hard_PWM_ini1, Hard_PWM_ini2, Hard_PWM_ini3, Hard_PWM_ini4;

	__TIM4_CLK_ENABLE();

	Timer.Instance = TIM4;

	Timer.Init.Prescaler = DEFAULT_TIM_PRESCALER;
	Timer.Init.CounterMode = TIM_COUNTERMODE_UP;
	Timer.Init.Period = DEFAULT_TIM_PERIOD;

	HAL_TIM_Base_Init(&Timer);
	HAL_TIM_PWM_Init(&Timer);

	Hard_PWM_ini1.OCMode = TIM_OCMODE_PWM1;
	Hard_PWM_ini1.Pulse = DEFAULT_TIM_PERIOD / div;
	Hard_PWM_ini1.OCPolarity = TIM_OCNPOLARITY_HIGH;
	Hard_PWM_ini1.OCFastMode = TIM_OCFAST_ENABLE;
	HAL_TIM_PWM_ConfigChannel(&Timer, &Hard_PWM_ini1, TIM_CHANNEL_1);

	Hard_PWM_ini2.OCMode = TIM_OCMODE_PWM1;
	Hard_PWM_ini2.Pulse = DEFAULT_TIM_PERIOD / div;
	Hard_PWM_ini2.OCPolarity = TIM_OCNPOLARITY_HIGH;
	Hard_PWM_ini2.OCFastMode = TIM_OCFAST_ENABLE;
	HAL_TIM_PWM_ConfigChannel(&Timer, &Hard_PWM_ini2, TIM_CHANNEL_2);

	Hard_PWM_ini3.OCMode = TIM_OCMODE_PWM1;
	Hard_PWM_ini3.Pulse = DEFAULT_TIM_PERIOD / div;
	Hard_PWM_ini3.OCPolarity = TIM_OCNPOLARITY_HIGH;
	Hard_PWM_ini3.OCFastMode = TIM_OCFAST_ENABLE;
	HAL_TIM_PWM_ConfigChannel(&Timer, &Hard_PWM_ini3, TIM_CHANNEL_3);

	Hard_PWM_ini4.OCMode = TIM_OCMODE_PWM1;
	Hard_PWM_ini4.Pulse = DEFAULT_TIM_PERIOD / div;
	Hard_PWM_ini4.OCPolarity = TIM_OCNPOLARITY_HIGH;
	Hard_PWM_ini4.OCFastMode = TIM_OCFAST_ENABLE;
	HAL_TIM_PWM_ConfigChannel(&Timer, &Hard_PWM_ini4, TIM_CHANNEL_4);
}

void InitializeLED(GPIO_InitTypeDef *L)
{
	int i;

    __GPIOD_CLK_ENABLE();

    L[0].Pin = GPIO_PIN_12;
    L[1].Pin = GPIO_PIN_13;
    L[2].Pin = GPIO_PIN_14;
    L[3].Pin = GPIO_PIN_15;

    for(i = 0; i < LED_NUM; i++)
    {
    	L[i].Mode = GPIO_MODE_AF_PP;
    	L[i].Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    	L[i].Pull = GPIO_NOPULL;
    	L[i].Alternate = GPIO_AF2_TIM4;

		HAL_GPIO_Init(GPIOD, &L[i]);
    }
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
