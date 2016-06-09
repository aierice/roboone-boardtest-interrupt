#include "stm32f4xx.h"
#include "usart_handler.h"
#include "make_motion.h"
#include "initialsetting.h"
/*
void errorLED_composed( void){
	while(1){
			GPIO_SetBits(GPIOA,GPIO_Pin_11);
			tdelay(900);
			GPIO_ResetBits(GPIOA,GPIO_Pin_11);
			tdelay(100);
		}
}
*/
