#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "diag/Trace.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

void ConfigureADC(void);
void InitPA5(void);
void InitPA1(void);

ADC_HandleTypeDef ADCini;
GPIO_InitTypeDef PA5ini, PA1ini;
int ADCval;

int main(int argc, char* argv[])
{
	ConfigureADC();

	while(1)
	{
		HAL_ADC_Start(&ADCini);
		while(!(ADC1->SR & ADC_SR_EOC));
		ADCval = ADC1->DR;
	}
}

void InitPA5(void)
{
	//__GPIOA_CLK_ENABLE();

	PA5ini.Pin = GPIO_PIN_5;
	PA5ini.Mode = GPIO_MODE_OUTPUT_PP;
	PA5ini.Speed = GPIO_SPEED_FREQ_LOW;
	PA5ini.Pull = GPIO_PULLUP;

	HAL_GPIO_Init(GPIOA, &PA5ini);

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
}

void InitPA1(void)
{
	//__GPIOA_CLK_ENABLE();

	PA1ini.Pin = GPIO_PIN_1;
	PA1ini.Mode = GPIO_MODE_ANALOG;
	PA1ini.Pull = GPIO_PULLUP;

	HAL_GPIO_Init(GPIOA, &PA1ini);
}

void ConfigureADC(void)
{
//    GPIO_InitTypeDef gpioInit;

    __GPIOA_CLK_ENABLE();
    __ADC1_CLK_ENABLE();

    InitPA5();
    InitPA1();

//    gpioInit.Pin = GPIO_PIN_1;
//    gpioInit.Mode = GPIO_MODE_ANALOG;
//    gpioInit.Pull = GPIO_PULLUP;
//
//    HAL_GPIO_Init(GPIOA, &gpioInit);

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

    ADC_ChannelConfTypeDef adcChannel;

    adcChannel.Channel = ADC_CHANNEL_1;
    adcChannel.Rank = 1;
    adcChannel.SamplingTime = ADC_SAMPLETIME_144CYCLES;
    adcChannel.Offset = 0;

    HAL_ADC_ConfigChannel(&ADCini, &adcChannel);
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
