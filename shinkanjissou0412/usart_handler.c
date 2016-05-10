#include "stm32f4xx.h"
#include "usart_handler.h"
#include "make_motion.h"
#include "initialsetting.h"

void USART3_IRQHandler( void){
	uint16_t command = 0;
	static uint16_t commandfull = 0;
	if( USART_GetITStatus( USART3,USART_IT_RXNE) != RESET){
		USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
		USART_ClearITPendingBit( USART3, USART_IT_RXNE);
/*		command = ( uint8_t)USART_ReceiveData( USART3);
		if(command & 0b0000000010000000 || !commandfull){
			commandfull = command<<8;
			GPIO_ResetBits( GPIOA,GPIO_Pin_11);
		}
		else if(~command & 0b0000000010000000 || commandfull){
			commandfull = commandfull ^ command;
			do_motion( commandfull);
			GPIO_SetBits( GPIOA,GPIO_Pin_11);
			commandfull = 0;
		}
		else{
			commandfull = 0;
			errorLED_composed();
		}
*/
/*		while(1){
			GPIO_SetBits( GPIOA,GPIO_Pin_11);
			tdelay(300);
			GPIO_ResetBits( GPIOA,GPIO_Pin_11);
			tdelay(300);
		}
*/
		command = ( uint8_t)USART_ReceiveData( USART3);
		if(command == 0b10000001){
			GPIO_ResetBits(GPIOA,GPIO_Pin_11);
//			tdelay(1);
//			GPIO_ResetBits(GPIOA,GPIO_Pin_11);
		}
		else{
		}
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	}
}

void errorLED_composed( void){
	while(1){
			GPIO_SetBits(GPIOA,GPIO_Pin_11);
			tdelay(900);
			GPIO_ResetBits(GPIOA,GPIO_Pin_11);
			tdelay(100);
		}
}
