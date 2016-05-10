#include"stm32f4xx.h"
#include"initialsetting.h"
#include"make_motion.h"

void send_exp_USART1(void);

int main(void)
{
	SystemInit();
	GPIO_Configuration();
	NVIC_Configuration();
	USART1_Configuration();
	USART3_Configuration();

	if(SysTick_Config(SystemCoreClock / 1000)){
		while(1);
	}

//	do_motion(0b0111111111111111);
	while(1)
    {
		while(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0));
		send_exp_USART1();
    	tdelay(500);
    	GPIO_SetBits(GPIOA,GPIO_Pin_11);
    }
}

void send_exp_USART1( void){
	while( USART_GetFlagStatus( USART1,USART_FLAG_TXE)==RESET);
	USART_SendData( USART1,0b10000001);
	tdelay(1000);
}
