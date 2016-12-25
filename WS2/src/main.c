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

#define LED_NUM 4

int led_ind = 0;
int direction = 1;
TIM_HandleTypeDef Timer;
GPIO_InitTypeDef LED[LED_NUM];

void InitializeTimer(void);
void InitializeLED(GPIO_InitTypeDef*);
void InitializeButton(void);
int UpdateInd(int);
void ChangeDirection(void);

int main(int argc, char* argv[])
{
	InitializeLED(LED);
	InitializeTimer();
	InitializeButton();

	HAL_NVIC_EnableIRQ(TIM2_IRQn);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);

	// Infinite loop
	while (1)
	{
	}
}

void InitializeTimer(void)
{
	RCC->CFGR &= ~(RCC_CFGR_SW);
	RCC->CFGR |= RCC_CFGR_SW_HSE;

    __TIM2_CLK_ENABLE();

    Timer.Instance = TIM2;

    Timer.Init.Prescaler = 4000 - 1;
    Timer.Init.CounterMode = TIM_COUNTERMODE_UP;
    Timer.Init.Period = 1000;

    HAL_TIM_Base_Init(&Timer);
    HAL_TIM_Base_Start_IT(&Timer);
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
    	L[i].Mode = GPIO_MODE_OUTPUT_PP;
    	L[i].Speed = GPIO_SPEED_HIGH;
    	L[i].Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOD, &L[i]);
    }

    HAL_GPIO_TogglePin(GPIOD, LED[0].Pin);
}

void InitializeButton(void)
{

	GPIO_InitTypeDef UserButton;

	__GPIOA_CLK_ENABLE();

	UserButton.Mode = GPIO_MODE_IT_RISING;
	UserButton.Speed = GPIO_SPEED_FREQ_LOW;
	UserButton.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOA, &UserButton);
}

int UpdateInd(int direction)
{
	int Y = 0;

	if(direction)
	{
		led_ind++;
		led_ind %= LED_NUM;
	}
	else
	{
		led_ind--;
		led_ind += LED_NUM;
		led_ind %= LED_NUM;
	}

	return Y;
}

void ChangeDirection(void)
{
	direction ? (direction = 0) : (direction = 1);
}

void TIM2_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&Timer);

    HAL_GPIO_TogglePin(GPIOD, LED[led_ind].Pin);
	UpdateInd(direction);
	HAL_GPIO_TogglePin(GPIOD, LED[led_ind].Pin);
}

void EXTI0_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);

	ChangeDirection();
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
