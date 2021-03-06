#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "diag/Trace.h"

#define LED_NUM 				(4)

#define DEFAULT_FREQUENCY		(50.0)

#define DEFAULT_TIM_PRESCALER	(4)
#define DEFAULT_TIM_PERIOD		(((HSE_VALUE / 2.0) / DEFAULT_TIM_PRESCALER) / DEFAULT_FREQUENCY)

void SystemClockConfig(void);
void HardPWM(int);
void InitializeLED(GPIO_InitTypeDef*);
void InitializeButton(void);
void ChangeBritness(void);

TIM_HandleTypeDef Timer;
GPIO_InitTypeDef LED[LED_NUM];

float LEDbritness = DEFAULT_TIM_PERIOD / 10;

int main()
{
	HardPWM(LEDbritness);
	InitializeLED(LED);
	InitializeButton();

	HAL_NVIC_EnableIRQ(EXTI0_IRQn);

	HAL_TIM_Base_Start(&Timer);
	HAL_TIM_PWM_Start(&Timer, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&Timer, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&Timer, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&Timer, TIM_CHANNEL_4);


	while(1);
}

void SystemClockConfig(void)
{
	RCC->CFGR &= ~(RCC_CFGR_SW);
	RCC->CFGR |= RCC_CFGR_SW_HSE;
}

void HardPWM(int imp_len)
{
	TIM_OC_InitTypeDef Hard_PWM_ini1, Hard_PWM_ini2, Hard_PWM_ini3, Hard_PWM_ini4;

	SystemClockConfig();

	__TIM4_CLK_ENABLE();

	Timer.Instance = TIM4;

	Timer.Init.Prescaler = DEFAULT_TIM_PRESCALER - 1;
	Timer.Init.CounterMode = TIM_COUNTERMODE_UP;
	Timer.Init.Period = DEFAULT_TIM_PERIOD;

	HAL_TIM_Base_Init(&Timer);
	HAL_TIM_PWM_Init(&Timer);

	Hard_PWM_ini1.OCMode = TIM_OCMODE_PWM1;
	Hard_PWM_ini1.Pulse = imp_len;
	Hard_PWM_ini1.OCPolarity = TIM_OCNPOLARITY_HIGH;
	Hard_PWM_ini1.OCFastMode = TIM_OCFAST_ENABLE;
	HAL_TIM_PWM_ConfigChannel(&Timer, &Hard_PWM_ini1, TIM_CHANNEL_1);

	Hard_PWM_ini2.OCMode = TIM_OCMODE_PWM1;
	Hard_PWM_ini2.Pulse = imp_len;
	Hard_PWM_ini2.OCPolarity = TIM_OCNPOLARITY_HIGH;
	Hard_PWM_ini2.OCFastMode = TIM_OCFAST_ENABLE;
	HAL_TIM_PWM_ConfigChannel(&Timer, &Hard_PWM_ini2, TIM_CHANNEL_2);

	Hard_PWM_ini3.OCMode = TIM_OCMODE_PWM1;
	Hard_PWM_ini3.Pulse = imp_len;
	Hard_PWM_ini3.OCPolarity = TIM_OCNPOLARITY_HIGH;
	Hard_PWM_ini3.OCFastMode = TIM_OCFAST_ENABLE;
	HAL_TIM_PWM_ConfigChannel(&Timer, &Hard_PWM_ini3, TIM_CHANNEL_3);

	Hard_PWM_ini4.OCMode = TIM_OCMODE_PWM1;
	Hard_PWM_ini4.Pulse = imp_len;
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

void InitializeButton(void)
{

	GPIO_InitTypeDef UserButton;

	__GPIOA_CLK_ENABLE();

	UserButton.Pin = GPIO_PIN_0;
	UserButton.Mode = GPIO_MODE_IT_RISING;
	UserButton.Speed = GPIO_SPEED_FREQ_LOW;
	UserButton.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOA, &UserButton);

}

void ChangeBritness(void)
{
	float temp = LEDbritness * 0.2;
	LEDbritness += temp;

	if(LEDbritness < DEFAULT_TIM_PERIOD)
	{
		__HAL_TIM_SET_COMPARE(&Timer, TIM_CHANNEL_1, LEDbritness);
		__HAL_TIM_SET_COMPARE(&Timer, TIM_CHANNEL_2, LEDbritness);
		__HAL_TIM_SET_COMPARE(&Timer, TIM_CHANNEL_3, LEDbritness);
		__HAL_TIM_SET_COMPARE(&Timer, TIM_CHANNEL_4, LEDbritness);
	}
	else
	{
		LEDbritness = DEFAULT_TIM_PERIOD / 10.0;
		__HAL_TIM_SET_COMPARE(&Timer, TIM_CHANNEL_1, LEDbritness);
		__HAL_TIM_SET_COMPARE(&Timer, TIM_CHANNEL_2, LEDbritness);
		__HAL_TIM_SET_COMPARE(&Timer, TIM_CHANNEL_3, LEDbritness);
		__HAL_TIM_SET_COMPARE(&Timer, TIM_CHANNEL_4, LEDbritness);
	}
}

//---------------------------------------------------------------//

void EXTI0_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);

	ChangeBritness();
}
