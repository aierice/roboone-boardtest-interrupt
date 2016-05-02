#include "stm32f4xx.h"
#include "usart_handler.h"
#include "make_motion.h"
#include "initialsetting.h"

void USART3_IRQHandler( void){
	uint16_t command = 0;
	static uint16_t commandfull = 0;
	if( USART_GetITStatus( USART3,USART_IT_RXNE) != RESET){
//		USART_ClearFlag( USART3,USART_FLAG_RXNE);			//0502これいらなさそう
		USART_ClearITPendingBit( USART3, USART_IT_RXNE);	//0502追加．でもこれ書いてないサンプルもあるし……
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
		while(1){
			GPIO_SetBits( GPIOA,GPIO_Pin_11);
			tdelay(300);
			GPIO_ResetBits( GPIOA,GPIO_Pin_11);
			tdelay(300);
		}
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
