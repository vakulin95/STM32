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
#define DEFAULT_TIM_PERIOD		(1000.0)
#define DEFAULT_TIM_PRESCALER	(0)

void SystemClockConfig(void);
void HardPWM(float);
void InitializeLED(GPIO_InitTypeDef*);

TIM_HandleTypeDef Timer;
GPIO_InitTypeDef LED[LED_NUM];

int main(int argc, char* argv[])
{
	int i;

	//SystemClockConfig();
	HardPWM(50);
	InitializeLED(LED);

	HAL_TIM_Base_Start(&Timer);
	HAL_TIM_PWM_Start(&Timer, TIM_CHANNEL_4);

	while(1)
	{
		//for(i = 0; i < 1000000; i++);
		//HAL_GPIO_TogglePin(GPIOD, LED[0].Pin);
		//for(i = 0; i < 1000000; i++);
	}
}

void SystemClockConfig(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  //RCC_ClkInitTypeDef RCC_ClkInitStruct;

  //__PWR_CLK_ENABLE();

  //__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

//  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1
//                              |RCC_CLOCKTYPE_PCLK2;
//  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
//  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
//  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
//  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);
//
//  HAL_RCC_EnableCSS();

}

void HardPWM(float div)
{
//	RCC->CFGR &= ~(RCC_CFGR_SW);
//	RCC->CFGR |= RCC_CFGR_SW_HSE;

	TIM_ClockConfigTypeDef sClockSourceConfig;
//	TIM_MasterConfigTypeDef sMasterConfig;

	TIM_OC_InitTypeDef Hard_PWM_ini;

	__TIM4_CLK_ENABLE();

	Timer.Instance = TIM4;

	Timer.Init.Prescaler = DEFAULT_TIM_PRESCALER;
	Timer.Init.CounterMode = TIM_COUNTERMODE_UP;
	Timer.Init.Period = DEFAULT_TIM_PERIOD;

	HAL_TIM_Base_Init(&Timer);

//	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
//	HAL_TIM_ConfigClockSource(&Timer, &sClockSourceConfig);

	HAL_TIM_PWM_Init(&Timer);

//	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
//	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
//	HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig);

	Hard_PWM_ini.OCMode = TIM_OCMODE_PWM1;
	Hard_PWM_ini.Pulse = DEFAULT_TIM_PERIOD / div;
	Hard_PWM_ini.OCPolarity = TIM_OCNPOLARITY_HIGH;
	Hard_PWM_ini.OCFastMode = TIM_OCFAST_ENABLE;

	HAL_TIM_PWM_ConfigChannel(&Timer, &Hard_PWM_ini, TIM_CHANNEL_4);

//	HAL_TIM_Base_Start_IT(&Timer);
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
    	//L[i].Mode = GPIO_MODE_OUTPUT_PP;
    	L[i].Mode = GPIO_MODE_AF_PP;
    	L[i].Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    	L[i].Pull = GPIO_NOPULL;
    	L[i].Alternate = GPIO_AF2_TIM4;

		HAL_GPIO_Init(GPIOD, &L[i]);
    }

//  HAL_GPIO_TogglePin(GPIOD, LED[0].Pin);
    HAL_GPIO_WritePin(GPIOD, LED[0].Pin, GPIO_PIN_SET);
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
