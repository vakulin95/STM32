#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "diag/Trace.h"

#define SWITCH_DELAY 	(1000000)

#define SW_GREEN() 		(GPIOD->ODR |= GPIO_ODR_ODR_12)
#define SW_ORANGE() 	(GPIOD->ODR |= GPIO_ODR_ODR_13)
#define SW_RED() 		(GPIOD->ODR |= GPIO_ODR_ODR_14)
#define SW_BLUE() 		(GPIOD->ODR |= GPIO_ODR_ODR_15)
#define LED_OFF() 		(GPIOD->ODR = 0)

void PLLConf(int);
void LEDInitialize(void);
void LEDLight(void);
void ButInitialize(void);
void SLKChange(void);

int main()
{
	int i;

	PLLConf(4);

	LEDInitialize();
	ButInitialize();

	while(1)
	{
		if(GPIOA->IDR & GPIO_IDR_IDR_0)
		{
			SW_RED();
			for(i = 0; i < SWITCH_DELAY; i++)
				;
			LED_OFF();

			SLKChange();
			LEDLight();
		}
		else
			LEDLight();
	}
}

void PLLConf(int pllp)
{
	if(pllp == 2)
	{
		RCC->CFGR &= ~(RCC_CFGR_SW);
		RCC->CFGR |= RCC_CFGR_SW_HSE;

		RCC->CR &= ~(RCC_CR_PLLON);
		RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLP);
		RCC->CR |= RCC_CR_PLLON;

		RCC->CFGR &= ~(RCC_CFGR_SW);
		RCC->CFGR |= RCC_CFGR_SW_PLL;
		SystemCoreClockUpdate();
	}
	if(pllp == 4)
	{
		RCC->CFGR &= ~(RCC_CFGR_SW);
		RCC->CFGR |= RCC_CFGR_SW_HSE;

		RCC->CR &= ~(RCC_CR_PLLON);
		RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLP);
		RCC->PLLCFGR |= RCC_PLLCFGR_PLLP_0;
		RCC->CR |= RCC_CR_PLLON;

		RCC->CFGR &= ~(RCC_CFGR_SW);
		RCC->CFGR |= RCC_CFGR_SW_PLL;
		SystemCoreClockUpdate();
	}
	return;
}

void LEDInitialize(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD->MODER |= GPIO_MODER_MODER15_0;
	GPIOD->MODER |= GPIO_MODER_MODER14_0;
	GPIOD->MODER |= GPIO_MODER_MODER13_0;
	GPIOD->MODER |= GPIO_MODER_MODER12_0;
	GPIOD->OTYPER = 0;
	GPIOD->OSPEEDR = 0;
}

void LEDLight(void)
{
	int i;

	SW_GREEN();
	for(i = 0; i < SWITCH_DELAY; i++);
	LED_OFF();

	SW_ORANGE();
	for(i = 0; i < SWITCH_DELAY; i++);
	LED_OFF();

	SW_RED();
	for(i = 0; i < SWITCH_DELAY; i++);
	LED_OFF();

	SW_BLUE();
	for(i = 0; i < SWITCH_DELAY; i++);
	LED_OFF();
}

void ButInitialize(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->OTYPER = 0;
	GPIOA->OSPEEDR = 0;
	GPIOA->PUPDR = 0;
}

void SLKChange(void)
{
	if((RCC->CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_HSI)
	{
		RCC->CFGR &= ~(RCC_CFGR_SW);
		RCC->CFGR |= RCC_CFGR_SW_HSE;
	}
	else
	{
		RCC->CFGR &= ~(RCC_CFGR_SW);
		RCC->CFGR |= RCC_CFGR_SW_HSI;
	}
}
