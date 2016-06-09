#ifndef USART3_H_
#define USART3_H_

uint32_t *RxBuff;
uint16_t commandfull = 0;
uint16_t precommandfull = 0;
void USART3_Configuration(void );
void errorLED_composed(void );

#endif
