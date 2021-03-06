#include "libs.h"

void ConfigureADC(void);
void ConfigureDAC(void);

void SetDAC(void);
int GetADC(int ADCval);

ADC_HandleTypeDef ADCini;
ADC_ChannelConfTypeDef ADCchan;
DAC_HandleTypeDef DACini;

int DACval = 0;
int dir = 1;

int main()
{
	ConfigurePins();
	ConfigureTIM2();
	ConfigureLED(LED);

	ConfigureADC();
	ConfigureDAC();

	HAL_NVIC_EnableIRQ(TIM2_IRQn);

	while(1);
}

void ConfigureADC(void)
{
    __ADC1_CLK_ENABLE();

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

void ConfigureDAC(void)
{
	__DAC_CLK_ENABLE();

	DACini.Instance = DAC;

	HAL_DAC_Init(&DACini);
}

void SetDAC(void)
{
	HAL_DAC_SetValue(&DACini, DAC_CHANNEL_2, DAC_ALIGN_8B_R, DACval);
	HAL_DAC_Start(&DACini, DAC_CHANNEL_2);

	dir ? DACval++ : DACval--;

	if(DACval <= 0 || DACval >= 255)
		dir = !dir;
}

int GetADC(int ADCval)
{
	int Y = 0;

	if(ADCval >= 0 && ADCval < 51)
	{
		goto ret;
	}
	else if(ADCval >= 51 && ADCval < 102)
	{
		Y = 1;
		goto ret;
	}
	else if(ADCval >= 102 && ADCval < 153)
	{
		Y = 2;
		goto ret;
	}
	else if(ADCval >= 153 && ADCval < 204)
	{
		Y = 3;
		goto ret;
	}
	else if(ADCval >= 204 && ADCval <= 255)
	{
		Y = 4;
		goto ret;
	}

ret:
	return Y;
}

//Interrupt handlers
//------------------------------------------------------//

void TIM2_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM2ini);

	SetDAC();

	HAL_ADC_Start(&ADCini);
	while(!(ADC1->SR & ADC_SR_EOC));
	SwitchLED(LED, GetADC(ADC1->DR));
}
