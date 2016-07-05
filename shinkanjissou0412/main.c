#include"stm32f4xx.h"
#include"initialsetting.h"
#include"make_motion.h"
#include"millis.h"
#include"usart3.h"

void send_exp_USART3(void);
extern uint8_t DMA2flag;
extern uint16_t commandfull;
extern uint16_t precommandfull;


int main(void)
{
	SystemInit();
	GPIO_Configuration();
	NVIC_Configuration();
	USART1_Configuration();
	USART3_Configuration();
	TIM3_Configuration();

	if(SysTick_Config(SystemCoreClock / 1000)){
		while(1);
	}
	GPIO_SetBits(GPIOA,GPIO_Pin_11);
	do_motion(0b0111111111111111);
	tdelay(10);
	while(1)
    {
//		while(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)){;
//		}
		if(DMA2flag == 1)GPIO_SetBits(GPIOA,GPIO_Pin_11);
		else GPIO_ResetBits(GPIOA,GPIO_Pin_11);
		millis_test();
		send_exp_USART3();
//		tdelay(10);
    }
}

void send_exp_USART3( void){
	while( USART_GetFlagStatus( USART3,USART_FLAG_TXE)==RESET);
	USART_SendData( USART3,0b10000010);
	while( USART_GetFlagStatus( USART3,USART_FLAG_TXE)==RESET);
	USART_SendData( USART3,0b00000010);
}
