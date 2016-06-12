#include"stm32f4xx.h"
#include"initialsetting.h"
#include"make_motion.h"
#include"millis.h"
#include"usart3.h"

void send_exp_USART1(void);
extern uint8_t DMA2flag;

int main(void)
{
	SystemInit();
	GPIO_Configuration();
	NVIC_Configuration();
	USART1_Configuration();
//	USART3_Configuration();
	TIM3_Configuration();

	if(SysTick_Config(SystemCoreClock / 1000)){
		while(1);
	}
	GPIO_SetBits(GPIOA,GPIO_Pin_11);
	do_motion(0b0111111111111111);
	while(DMA2flag){
	}
	GPIO_ResetBits(GPIOA,GPIO_Pin_11);
	while(1)
    {
/*		do_motion(0b0111111111111111);
		tdelay(1000);
		do_motion(0b1000000100000001);
		GPIO_SetBits(GPIOA,GPIO_Pin_11);
		tdelay(1000);
*/
/*		while(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0));
		do_motion(0b1000001000000010);
		GPIO_SetBits(GPIOA,GPIO_Pin_11);
		tdelay(500);
    	GPIO_ResetBits(GPIOA,GPIO_Pin_11);
    	tdelay(500);
*/
		millis_test();
//		do_motion(0b0111111111111111);
    }
}

void send_exp_USART1( void){
	while( USART_GetFlagStatus( USART1,USART_FLAG_TXE)==RESET);
	USART_SendData( USART1,0b10000001);
	tdelay(1000);
	while( USART_GetFlagStatus( USART1,USART_FLAG_TXE)==RESET);
	USART_SendData( USART1,0b10100001);
	tdelay(1000);
}
