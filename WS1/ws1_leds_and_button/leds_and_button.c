#include "leds_and_button.h"

#define SWITCH_DELAY 500000


#define SET_SYSCLOCK_SRC(_SW, _SWS) \
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW)); \
    RCC->CFGR |= (_SW); \
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != (_SWS)) {}


void clock_toggle(void)
{
  if ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) == RCC_CFGR_SWS_HSI)
  {
    SET_SYSCLOCK_SRC(RCC_CFGR_SW_HSE, RCC_CFGR_SWS_HSE);
  }
  else
  {
    SET_SYSCLOCK_SRC(RCC_CFGR_SW_HSI, RCC_CFGR_SWS_HSI);
  }
}


GPIO_InitTypeDef  GPIO_InitStructure;

#ifdef USE_EXT_LEDS
GPIO_TypeDef*   leds_gpio_ports[LEDn] = {LED1_GPIO_PORT,
                                         LED2_GPIO_PORT,
                                         LED3_GPIO_PORT,
                                         LED4_GPIO_PORT,
                                         LED5_GPIO_PORT,
                                         LED6_GPIO_PORT,
                                         LED7_GPIO_PORT,
                                         LED8_GPIO_PORT};

uint16_t  leds_gpio_pins[LEDn]  = {LED1_PIN,
                                   LED2_PIN,
                                   LED3_PIN,
                                   LED4_PIN,
                                   LED5_PIN,
                                   LED6_PIN,
                                   LED7_PIN,
                                   LED8_PIN};

const uint32_t  leds_gpio_clks[LEDn]  = {LED1_GPIO_CLK,
                                         LED2_GPIO_CLK,
                                         LED3_GPIO_CLK,
                                         LED4_GPIO_CLK,
                                         LED5_GPIO_CLK,
                                         LED6_GPIO_CLK,
                                         LED7_GPIO_CLK,
                                         LED8_GPIO_CLK};

uint16_t  reverse_leds[LEDn] = {LED8, LED7, LED6, LED5, LED4, LED3, LED2, LED1};
uint16_t  straight_leds[LEDn] = {LED1, LED2, LED3, LED4, LED5, LED6, LED7, LED8};

#else
GPIO_TypeDef*   leds_gpio_ports[LEDn] = {LED1_GPIO_PORT,
                                         LED2_GPIO_PORT,
                                         LED3_GPIO_PORT,
                                         LED4_GPIO_PORT};

const uint16_t  leds_gpio_pins[LEDn]  = {LED1_PIN,
                                         LED2_PIN,
                                         LED3_PIN,
                                         LED4_PIN};

const uint32_t  leds_gpio_clks[LEDn]  = {LED1_GPIO_CLK,
                                         LED2_GPIO_CLK,
                                         LED3_GPIO_CLK,
                                         LED4_GPIO_CLK};

uint16_t  reverse_leds[LEDn] = {LED4, LED3, LED2, LED1};
uint16_t  straight_leds[LEDn] = {LED1, LED2, LED3, LED4};

#endif

int leds_count_to_switch = sizeof(straight_leds)/sizeof(straight_leds[0]);

static void switch_leds(uint16_t *leds, int leds_cnt)
{
  volatile int i;
  int j;

  for (j = 0; j < leds_cnt; j++)
  {
    leds_gpio_ports[leds[j]]->BSRRL = leds_gpio_pins[leds[j]];

    for (i=0; i < SWITCH_DELAY; i++) {};

    leds_gpio_ports[leds[j]]->BSRRH = leds_gpio_pins[leds[j]];
  }
  for (i=0; i < SWITCH_DELAY; i++) {};
}


static void init_led(uint16_t led_no)
{
  RCC_AHB1PeriphClockCmd(leds_gpio_clks[led_no], ENABLE);
  GPIO_InitStructure.GPIO_Pin   = leds_gpio_pins[led_no];
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(leds_gpio_ports[led_no], &GPIO_InitStructure);
}


int main(void)
{
  int reverse = 0;
  int i;

  /* Enable peripheral clock for GPIOA port */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  /* Init leds */
  for (i = 0; i < LEDn; i++)
  {
    init_led(i);
  }

  /* Init button */
  GPIO_InitStructure.GPIO_Pin = WAKEUP_BUTTON_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  while (1)
  {
    reverse = GPIOA->IDR & WAKEUP_BUTTON_PIN;

    if (!reverse)
    {
      switch_leds(straight_leds, LEDn);
    }
    else
    {
      switch_leds(reverse_leds, LEDn);
    }
  }
}
