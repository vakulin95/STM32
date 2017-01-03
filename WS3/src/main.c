#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "diag/Trace.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

#define LED_NUM 				(4)

void InitPA5(void);
void InitPA1(void);
void ConfigureADC(void);
void InitLED(GPIO_InitTypeDef *L);
int GetADC(int ADCval);
void SwitchLED(GPIO_InitTypeDef *L, int N);

ADC_HandleTypeDef ADCini;
ADC_ChannelConfTypeDef ADCchan;
GPIO_InitTypeDef PA5ini, PA1ini;
GPIO_InitTypeDef LED[LED_NUM];

int main(int argc, char* argv[])
{
	ConfigureADC();
	InitLED(LED);

	while(1)
	{
		HAL_ADC_Start(&ADCini);
		while(!(ADC1->SR & ADC_SR_EOC));
		SwitchLED(LED, GetADC(ADC1->DR));
	}
}

void InitPA5(void)
{
	PA5ini.Pin = GPIO_PIN_5;
	PA5ini.Mode = GPIO_MODE_OUTPUT_PP;
	PA5ini.Speed = GPIO_SPEED_FREQ_LOW;
	PA5ini.Pull = GPIO_PULLUP;

	HAL_GPIO_Init(GPIOA, &PA5ini);

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
}

void InitPA1(void)
{
	PA1ini.Pin = GPIO_PIN_1;
	PA1ini.Mode = GPIO_MODE_ANALOG;
	PA1ini.Pull = GPIO_PULLUP;

	HAL_GPIO_Init(GPIOA, &PA1ini);
}

void ConfigureADC(void)
{
    __GPIOA_CLK_ENABLE();
    __ADC1_CLK_ENABLE();

    InitPA5();	// output
    InitPA1();	// input

    ADCini.Instance = ADC1;
    ADCini.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
    ADCini.Init.Resolution = ADC_RESOLUTION_8B;
    ADCini.Init.ScanConvMode = DISABLE;
    ADCini.Init.ContinuousConvMode = ENABLE;
    ADCini.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    ADCini.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
    ADCini.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    ADCini.Init.NbrOfConversion = 1;

    HAL_ADC_Init(&ADCini);

    ADCchan.Channel = ADC_CHANNEL_1;
    ADCchan.Rank = 1;
    ADCchan.SamplingTime = ADC_SAMPLETIME_144CYCLES;
    ADCchan.Offset = 0;

    HAL_ADC_ConfigChannel(&ADCini, &ADCchan);
}

void InitLED(GPIO_InitTypeDef *L)
{
	int i;

    __GPIOD_CLK_ENABLE();

    L[0].Pin = GPIO_PIN_12;
    L[1].Pin = GPIO_PIN_13;
    L[2].Pin = GPIO_PIN_14;
    L[3].Pin = GPIO_PIN_15;

    for(i = 0; i < LED_NUM; i++)
    {
    	L[i].Mode = GPIO_MODE_OUTPUT_PP ;
    	L[i].Speed = GPIO_SPEED_FREQ_LOW;
    	L[i].Pull = GPIO_NOPULL;

		HAL_GPIO_Init(GPIOD, &L[i]);
    }
}

int GetADC(int ADCval)
{
	int Y = 0;

	if(!ADCval)
	{
		goto ret;
	}
	else if(ADCval > 0 && ADCval < 64)
	{
		Y = 1;
		goto ret;
	}
	else if(ADCval >= 64 && ADCval < 128)
	{
		Y = 2;
		goto ret;
	}
	else if(ADCval >= 128 && ADCval < 192)
	{
		Y = 3;
		goto ret;
	}
	else if(ADCval >= 192 && ADCval < 256)
	{
		Y = 4;
		goto ret;
	}

ret:
	return Y;
}

void SwitchLED(GPIO_InitTypeDef *L, int N)
{
	int i;

	for(i = 0; i < LED_NUM; i++)
	{
		HAL_GPIO_WritePin(GPIOD, L[i].Pin, GPIO_PIN_RESET);
	}

	for(i = 0; i < N; i++)
	{
		HAL_GPIO_WritePin(GPIOD, L[i].Pin, GPIO_PIN_SET);
	}
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
