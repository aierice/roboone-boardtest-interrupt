#include"stm32f4xx.h"
#include"initialsetting.h"

extern uint32_t RxBuff[];

void USART3_Configuration(void ){
	NVIC_InitTypeDef NVIC_InitStructure;
	// DMA���荞��(serveRx)�̐ݒ�
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//DMA1_Stream5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//USART3��RX�s����PC11�BTX�s����PC10
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

//	USART_ClearITPendingBit(USART3,USART_IT_RXNE);		//0502�ǉ�
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

/*	//Configure DMA for usart1
	DMA_InitTypeDef DMA_InitStructure;

	DMA_DeInit(DMA1_Stream1);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single ;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t) (&(USART3->DR)) ;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	// Configure TX DMA
	DMA_InitStructure.DMA_BufferSize = 1;		//�ق�܂��H
	DMA_InitStructure.DMA_Channel = DMA_Channel_4 ;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory ;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)RxBuff;
	DMA_Init(DMA1_Stream1,&DMA_InitStructure);
	DMA_Cmd(DMA1_Stream1,DISABLE);
//	// Configure RX DMA
//	DMA_InitStructure.DMA_BufferSize = 1;
//	DMA_InitStructure.DMA_Channel = DMA_Channel_4 ;
//	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory ;
//	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)ServoCommandData.RX_BUFFER ;
//	DMA_Init(DMA2_Stream5,&DMA_InitStructure);
//	DMA_Cmd(DMA2_Stream5,ENABLE);

	DMA_ITConfig(DMA1_Stream1, DMA_IT_TC, ENABLE);
//	DMA_ITConfig(DMA2_Stream5, DMA_IT_TC, ENABLE);

	USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);
//	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
*/
	USART_Cmd(USART3,ENABLE);
}

void DMA1_Stream1_IRQHandler (void) {
	GPIO_SetBits(GPIOA,GPIO_Pin_11);
//	if(RxBuff==1)GPIO_SetBits(GPIOA,GPIO_Pin_11);
//	else GPIO_ResetBits(GPIOA,GPIO_Pin_11);
}

void USART3_IRQHandler(void ){
	GPIO_SetBits(GPIOA,GPIO_Pin_11);
}