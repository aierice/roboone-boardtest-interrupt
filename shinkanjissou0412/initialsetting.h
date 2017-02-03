#ifndef INITIALSETTING_H_
#define INITIALSETTING_H_

void NVIC_Configuration(void);
void GPIO_Configuration(void);
void USART1_Configuration(void);
//void USART3_Configuration(void);
extern uint32_t timingdelay;
void tdelay(uint32_t nTime);

#endif
