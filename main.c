#include "main.h"
#include "ff.h"
#include "diskio.h"

/*
 * 
 * 
 */
volatile uint32_t utick = 0;

void timer_init(void);
void clock_init(void);
void led_init(void);
void SDTimer_Handler(void);

volatile uint8_t FatFsCnt = 0;
volatile uint16_t Timer1, Timer2;
void spi1_init(void);

int main(void) {  

    SystemInit();//clock to 24 Mhz
    SysTick_Config(23999);//interupt every 1 ms 
    led_init(); 
    spi1_init();   
    
    FATFS fs;
    FIL fp;
    FRESULT res;

    res = f_mount (&fs, "",0); 
    f_open(&fp,"stm32.txt",FA_OPEN_ALWAYS|FA_WRITE);
    f_printf(&fp,"hello stm32");
	  f_close(&fp);
     delay(50);
    f_open(&fp,"test.txt",FA_OPEN_ALWAYS|FA_WRITE);
    f_printf(&fp,"fat driver");
	  f_close(&fp); 
    
    while(1) { 
     
        GPIO_SetBits(GPIOC, GPIO_Pin_13);  // turn the LED on
        GPIO_SetBits(GPIOA, GPIO_Pin_0); 
        delay(300);

        GPIO_ResetBits(GPIOC, GPIO_Pin_13);  // turn the LED off
        GPIO_ResetBits(GPIOA, GPIO_Pin_0);
        delay(300);
    }
}


void led_init(void)
{
  // GPIO structure for port initialization
    GPIO_InitTypeDef GPIO_InitStructure;
   // enable clock on APB2
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,  ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,  ENABLE);
    // configure port A1 for driving an LED
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    // output push-pull mode
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;   // highest speed
    GPIO_Init(GPIOC, &GPIO_InitStructure) ; 
    GPIO_SetBits(GPIOC, GPIO_Pin_13);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    // output push-pull mode
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;   // highest speed
    GPIO_Init(GPIOA, &GPIO_InitStructure) ; 
}

void timer_init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    TIM_TimeBaseInitTypeDef timer1;
    timer1.TIM_Prescaler = 3000;
    timer1.TIM_CounterMode = TIM_CounterMode_Up;
    timer1.TIM_Period = 2000;
    timer1.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM2,&timer1);
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM2,ENABLE);
    NVIC_EnableIRQ(TIM2_IRQn);
}

void TIM2_IRQHandler(void)
{
  if(TIM_GetFlagStatus(TIM2, TIM_FLAG_Update))
  {
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    GPIO_WriteBit(GPIOC,GPIO_Pin_13,!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13));
  }
}


void spi1_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
   // enable clock on APB2
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,  ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 ,ENABLE);
    // configure port A7 et A5 for driving an LED
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    // output push-pull mode
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   // highest speed
    GPIO_Init(GPIOA, &GPIO_InitStructure) ;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    // output push-pull mode
    //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   // highest speed
    GPIO_Init(GPIOA, &GPIO_InitStructure) ; 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    // output push-pull mode
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   // highest speed
    GPIO_Init(GPIOA, &GPIO_InitStructure) ; 

    SPI_InitTypeDef spi1;
	  spi1.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    spi1.SPI_DataSize = SPI_DataSize_8b;
    spi1.SPI_Mode = SPI_Mode_Master ;
    spi1.SPI_CPOL = SPI_CPOL_Low;
    spi1.SPI_CPHA = SPI_CPHA_1Edge;
    spi1.SPI_NSS = SPI_NSS_Soft;
    spi1.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    spi1.SPI_FirstBit = SPI_FirstBit_MSB;
    spi1.SPI_CRCPolynomial = 10;
    SPI_Init(SPI1,&spi1);
    SPI_Cmd(SPI1,ENABLE);
}

void SDTimer_Handler(void)
{
  if(Timer1 > 0)
    Timer1--;

  if(Timer2 > 0)
    Timer2--;
}

void SysTick_Handler(void)
{
   FatFsCnt++;
	  if(FatFsCnt >= 10)
	  {
	    FatFsCnt = 0;
	    SDTimer_Handler();
	  }
  utick++;
}

uint32_t get_tick()
{
    return utick ; 
}

void delay(uint32_t wait)
{
    uint32_t st = utick ;
    while(utick - st < wait);
}
