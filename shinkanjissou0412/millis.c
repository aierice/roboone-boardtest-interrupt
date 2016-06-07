#include "stm32f4xx.h"
#include "initialsetting.h"

extern uint32_t period;

void TIM3_Configuration(void){
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	//84MHz
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;
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
	while(1){
		if(millis()%1000 >= 900)GPIO_ResetBits(GPIOA,GPIO_Pin_11);
		else GPIO_SetBits(GPIOA,GPIO_Pin_11);
/*
 * 		while(millis()<1000){
 * 			GPIO_ResetBits(GPIOA,GPIO_Pin_11);
 * 		}
 * 		period = 0;
 * 		GPIO_SetBits(GPIOA,GPIO_Pin_11);
 * 		tdelay(10);
 * */
	}
}

void TIM3_IRQHandler(void){
	period++;
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}
