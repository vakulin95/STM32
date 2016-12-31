#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "diag/Trace.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

void SystemClockConfig(void);
void ConfigureADC(void);
void InitPA5(void);

ADC_HandleTypeDef g_AdcHandle;
GPIO_InitTypeDef PA5ini;
int g_ADCValue;

int main(int argc, char* argv[])
{
	//SystemClockConfig();
	//InitPA5();

	ConfigureADC();

	while(1)
	{
		HAL_ADC_Start(&g_AdcHandle);
		while(!(ADC1->SR & ADC_SR_EOC)) {};
		//g_ADCValue = HAL_ADC_GetValue(&g_AdcHandle);
		g_ADCValue = ADC1->DR;
	}
}

void SystemClockConfig(void)
{
	RCC->CFGR &= ~(RCC_CFGR_SW);
	RCC->CFGR |= RCC_CFGR_SW_HSE;

	//SystemCoreClockUpdate();
}

void InitPA5(void)
{
	__GPIOA_CLK_ENABLE();

	PA5ini.Pin = GPIO_PIN_5;
	PA5ini.Mode = GPIO_MODE_OUTPUT_PP;
	PA5ini.Speed = GPIO_SPEED_FREQ_LOW;
	PA5ini.Pull = GPIO_PULLUP;

	HAL_GPIO_Init(GPIOA, &PA5ini);

	GPIO

	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	//HAL_GPIO_TogglePin(GPIOA, PA5ini.Pin);
}

void ConfigureADC(void)
{
    GPIO_InitTypeDef gpioInit;

    InitPA5();

    __GPIOA_CLK_ENABLE();
    __ADC1_CLK_ENABLE();

    gpioInit.Pin = GPIO_PIN_1;
    gpioInit.Mode = GPIO_MODE_ANALOG;
    gpioInit.Pull = GPIO_PULLUP;

    HAL_GPIO_Init(GPIOA, &gpioInit);

    g_AdcHandle.Instance = ADC1;
    g_AdcHandle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
    //ADC driver configuration
    g_AdcHandle.Init.Resolution = ADC_RESOLUTION_8B;
    g_AdcHandle.Init.ScanConvMode = DISABLE;
    g_AdcHandle.Init.ContinuousConvMode = ENABLE;
    g_AdcHandle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    g_AdcHandle.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
    g_AdcHandle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    g_AdcHandle.Init.NbrOfConversion = 1;


    HAL_ADC_Init(&g_AdcHandle);

    ADC_ChannelConfTypeDef adcChannel;

    adcChannel.Channel = ADC_CHANNEL_1;
    adcChannel.Rank = 1;
    adcChannel.SamplingTime = ADC_SAMPLETIME_144CYCLES;
    adcChannel.Offset = 0;

    HAL_ADC_ConfigChannel(&g_AdcHandle, &adcChannel);
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------