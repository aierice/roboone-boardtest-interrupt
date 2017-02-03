#include "stm32f4xx.h"
#include "initialsetting.h"
#include "make_motion.h"
#include "usart3.h"
#include "motion_define.h"

uint32_t period = 0;
uint32_t maxperiod = 10000000;

extern uint16_t commandfull;
extern uint16_t precommandfull;
extern uint8_t motionphase;

void TIM3_Configuration(void){
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	//84MHz
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_TimeBaseStructure.TIM_Period = 10 - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = 8400 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);

	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

	TIM_Cmd(TIM3, ENABLE);
}

uint32_t millis(void ){
	return period;
}

uint8_t migration_checker(uint16_t precommandfull,uint16_t nowcommandfull){
	int16_t myreturn = 0;
	switch(precommandfull){
		case adr_Walk_front:		//é¿ç€ÇÕÇ±ÇÍÇÕÇ†ÇËÇ¶Ç»Ç¢Ç™ÅCâ¬ì«ê´ÇÃÇΩÇﬂ
		case adr_Walk_front^0b0000000000010000:{
			switch(nowcommandfull){
				case adr_Walk_left:
					myreturn = adr_Walk_left^0b0000000000110000;
					break;
				case adr_Walk_right:
					myreturn = adr_Walk_right^0b0000000000110000;
					break;
				default:
					myreturn = precommandfull^0b0000000000100000;
					break;
			}
//			uint16_t p[2]={adr_Walk_left,adr_Walk_right};
//			uint8_t i;
//			for(i = 0;i < 2;i++)myreturn+=(nowcommandfull==p[i]);
			}
			break;
		default:
			myreturn = precommandfull^0b0000000000100000;
			break;
	}
	return myreturn;
}

void millis_test(void ){
	if(maxperiod == 10000000){
		switch(motionphase){
			case 0:
				precommandfull = commandfull;
				break;
			case 1:
				precommandfull = precommandfull | 0b0000000000010000;
				break;
			case 2:
				if((precommandfull&0b1111111111101111) == commandfull){
					precommandfull = precommandfull;
				}
				else{
					precommandfull = migration_checker(precommandfull,commandfull);
				}
				break;
			case 3:
				precommandfull = commandfull;
				break;
			case 4:
				precommandfull = precommandfull | 0b0000000000010000;
				break;
			default://GPIO_SetBits(GPIOA,GPIO_Pin_11);
				break;
		}
		do_motion(precommandfull);
//		do_motion(0b1000000110000001);
	}
/*	if(maxperiod == 10000000){
		precommandfull = commandfull;
		do_motion(precommandfull);
	}
*/
	else if(maxperiod <= period){
		do_motion(precommandfull);
//		do_motion(0b1000000110000001);
	}
}

void TIM3_IRQHandler(void){
	period++;
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}
