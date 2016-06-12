#include"stm32f4xx.h"
#include"initialsetting.h"

extern uint8_t sendbuf[10000];

void NVIC_Configuration( void){
	NVIC_InitTypeDef NVIC_InitStructure;
	//USARTの割り込み設定
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn | USART3_IRQn;	//USART1_IRQnはいらない？
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &NVIC_InitStructure);

	// DMA割り込み(serveTx)の設定
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//tdelayの割り込み設定
	NVIC_InitStructure.NVIC_IRQChannel = SysTick_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &NVIC_InitStructure);
}

void GPIO_Configuration( void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void USART1_Configuration( void){
	//USART1のRXピンはPB7。TXピンはPB6
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOB , GPIO_PinSource6 , GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOB , GPIO_PinSource7 , GPIO_AF_USART1);

	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure);

//	USART_ClearITPendingBit(USART1,USART_IT_RXNE);		//0502追加．
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	/* Configure DMA for usart1 */
	DMA_InitTypeDef DMA_InitStructure;

//	DMA_DeInit(DMA2_Stream5);
	DMA_DeInit(DMA2_Stream7);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);			//168MHz

	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single ;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t) (&(USART1->DR)) ;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	/* Configure TX DMA */
	DMA_InitStructure.DMA_BufferSize = 1;		//ほんまか？
	DMA_InitStructure.DMA_Channel = DMA_Channel_4 ;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral ;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)sendbuf;	//uint32_tで合っているらしい
	DMA_Init(DMA2_Stream7,&DMA_InitStructure);
	DMA_Cmd(DMA2_Stream7,DISABLE);
//	/* Configure RX DMA */
//	DMA_InitStructure.DMA_BufferSize = 1;
//	DMA_InitStructure.DMA_Channel = DMA_Channel_4 ;
//	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory ;
//	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)ServoCommandData.RX_BUFFER ;
//	DMA_Init(DMA2_Stream5,&DMA_InitStructure);
//	DMA_Cmd(DMA2_Stream5,ENABLE);

	DMA_ITConfig(DMA2_Stream7, DMA_IT_TC, ENABLE);
//	DMA_ITConfig(DMA2_Stream5, DMA_IT_TC, ENABLE);

	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
//	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);

	USART_Cmd(USART1,ENABLE);
}
/*
void USART3_Configuration( void){
	//USART3のRXピンはPC11。TXピンはPC10
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART3, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOC , GPIO_PinSource10 , GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOC , GPIO_PinSource11 , GPIO_AF_USART3);

	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3,&USART_InitStructure);

//	USART_ClearITPendingBit(USART3,USART_IT_RXNE);		//0502追加
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART3,ENABLE);
}
*/
void SysTick_Handler(void){
	if(timingdelay){
		timingdelay--;
	}
}
void tdelay(uint32_t nTime){
	timingdelay = nTime;
	while(timingdelay);
}
