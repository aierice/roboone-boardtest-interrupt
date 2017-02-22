#include "stm32f4xx.h"
#include "initialsetting.h"
#include "make_motion.h"
#include "usart3.h"
#include "motion_define.h"

uint32_t period = 0;
uint32_t maxperiod = 10000000;

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

uint16_t migration_checker(uint16_t precommandfull,uint16_t nowcommandfull){
	uint16_t ret = 0;
	switch(precommandfull){
		case adr_Walk_front:		//é¿ç€ÇÕÇ±ÇÍÇÕÇ†ÇËÇ¶Ç»Ç¢Ç™ÅCâ¬ì«ê´ÇÃÇΩÇﬂ
		case adr_Walk_front|0b0000000000010000:{
			switch(nowcommandfull){
				case adr_Walk_left:
				case adr_Walk_left_front:
					ret = adr_Walk_left|0b0000000000110000;
					break;
				case adr_Walk_right:
				case adr_Walk_right_front:
					ret = adr_Walk_right|0b0000000000110000;
						break;
				case adr_Atk_3:
					ret = adr_Atk_3|0b0000000000110000;
					break;
				default:
					ret = precommandfull|0b000000000100000;
					ret = ret&0b1111111111101111;
					break;
			}
		}
		break;
		case adr_Walk_left:
		case adr_Walk_left|0b00000000000010000:
			switch(nowcommandfull){
				case adr_Walk_front:
				case adr_Walk_left_front:
					ret = adr_Walk_front|0b0000000000110000;
					break;
				default:
					ret = precommandfull|0b000000000100000;
					ret = ret&0b1111111111101111;
					break;
			}
		break;
		case adr_Walk_right:
		case adr_Walk_right|0b00000000000010000:
			switch(nowcommandfull){
				case adr_Walk_front:
				case adr_Walk_right_front:
					ret = adr_Walk_front|0b0000000001110000;
					break;
				default:
					ret = precommandfull|0b000000000100000;
					ret = ret&0b1111111111101111;
					break;
			}
		break;


		case adr_test:
		case adr_test|0b00000000000010000:
			switch(nowcommandfull){
				case adr_test2:
					ret = adr_test2|0b0000000000110000;
					break;
				default:
					ret = precommandfull|0b000000000100000;
					ret = ret&0b1111111111101111;
//					ret = (0x8021);
					break;
			}
		break;
		default:
			ret = precommandfull|0b0000000000100000;
		break;
	}
//	GPIO_ResetBits(GPIOA,GPIO_Pin_11);
	return ret;
}

void millis_test(void ){
	uint16_t myreturn;
	if(maxperiod == 10000000){
		switch(motionphase){
			case 0:
				myreturn = commandfull;
				break;
			case 1:
				myreturn = precommandfull | 0b0000000000010000;
				break;
			case 2:
				if((precommandfull&0b1111111111101111) == commandfull){
					myreturn = precommandfull;
				}
				else{
					myreturn = migration_checker(precommandfull,commandfull);
//					if(myreturn==(0x8021)){
//								GPIO_SetBits(GPIOA,GPIO_Pin_11);
//								tdelay(100);
//								GPIO_ResetBits(GPIOA,GPIO_Pin_11);
//								tdelay(100);
//					}
				}
				break;
			case 3:
				myreturn = commandfull;
				break;
			case 4:
				myreturn = precommandfull & 0b1111111100001111;
				myreturn = myreturn|0b0000000000010000;
				break;
			default://GPIO_SetBits(GPIOA,GPIO_Pin_11);
				break;
		}
		precommandfull = myreturn;
		do_motion(myreturn);
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
