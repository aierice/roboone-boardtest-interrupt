#include"stm32f4xx.h"
#include"initialsetting.h"
#include"make_motion.h"
#include"millis.h"
#include"usart3.h"
#include"motion_define.h"

void send_exp_USART3(void);

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
//	do_motion(0b0111111111111111);
	do_motion(adr_torque_on);
	GPIO_ResetBits(GPIOA,GPIO_Pin_11);
	tdelay(10);
	while(1)
	{
		millis_test();
//		tdelay(10);
    }
}

void send_exp_USART3( void){
	if(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)){
		while( USART_GetFlagStatus( USART3,USART_FLAG_TXE)==RESET);
		USART_SendData( USART3,0b10000010);
		while( USART_GetFlagStatus( USART3,USART_FLAG_TXE)==RESET);
		USART_SendData( USART3,0b00000010);
	}
	else{
			while( USART_GetFlagStatus( USART3,USART_FLAG_TXE)==RESET);
			USART_SendData( USART3,0b10000001);
			while( USART_GetFlagStatus( USART3,USART_FLAG_TXE)==RESET);
			USART_SendData( USART3,0b00000001);
	}
}
