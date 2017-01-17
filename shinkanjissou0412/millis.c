#include "stm32f4xx.h"
#include "initialsetting.h"
#include "make_motion.h"
#include  "usart3.h"

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
					precommandfull = precommandfull | 0b0000000000100000;
					precommandfull = precommandfull & 0b1111111111101111;
				}
				break;
			case 3:
				precommandfull = commandfull;
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
