#include"stm32f4xx.h"
#include"initialsetting.h"

uint32_t *RxBuff;
uint16_t commandfull = 0b1000000000000000;//0b1000000010000000;
uint16_t precommandfull = 0b1000000000000000;//0b1000000010000000;

void USART3_Configuration(void ){
	NVIC_InitTypeDef NVIC_InitStructure;
	// DMA割り込み(serveRx)の設定
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//DMA1_Stream5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

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
	DMA_InitStructure.DMA_BufferSize = 1;		//ほんまか？
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
//	GPIO_SetBits(GPIOA,GPIO_Pin_11);
//	if(RxBuff==1)GPIO_SetBits(GPIOA,GPIO_Pin_11);
//	else GPIO_ResetBits(GPIOA,GPIO_Pin_11);
}

//void USART3_IRQHandler(void ){
//	GPIO_SetBits(GPIOA,GPIO_Pin_11);
//}

void USART3_IRQHandler( void){

	uint16_t command = 0;
	static uint16_t inpcommandfull;
	static uint8_t numoferror = 0;
	static uint8_t RXorder = 0;
	if( USART_GetITStatus( USART3,USART_IT_RXNE) != RESET){
		USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
		USART_ClearITPendingBit( USART3, USART_IT_RXNE);
		command = ( uint8_t)USART_ReceiveData( USART3);
		if(command & 0b0000000010000000){
			inpcommandfull = 0;		//いらないはず
			inpcommandfull = command<<8;
//			GPIO_ResetBits( GPIOA,GPIO_Pin_11);
			RXorder = 1;
		}
		else if(((~command) & 0b0000000010000000) && RXorder){
			inpcommandfull = inpcommandfull ^ command;
//			inpcommandfull = inpcommandfull ^ 0b0000000010000000;
			commandfull = inpcommandfull;
			numoferror = 0;
			RXorder = 0;
		}
		else{
			commandfull = precommandfull;
			inpcommandfull = commandfull;
			numoferror++;
			RXorder = 0;
		}
		if(numoferror >= 100){
			while(1)GPIO_SetBits(GPIOA,GPIO_Pin_11);
			commandfull = 0b1111111111111111;	//強制停止
		}
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	}


}


//2bitの受信を前提としたコントローラ．不要な機能っぽい
//	void USART3_IRQHandler( void){
//
//		uint16_t command = 0;
//		static uint16_t inpcommandfull;
//		static uint8_t numoferror = 0;
//		if( USART_GetITStatus( USART3,USART_IT_RXNE) != RESET){
//			USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
//			USART_ClearITPendingBit( USART3, USART_IT_RXNE);
//			command = ( uint8_t)USART_ReceiveData( USART3);
//			if(command & 0b0000000010000000){
//				inpcommandfull = 0;		//いらないはず
//				inpcommandfull = command<<8;
//	//			GPIO_ResetBits( GPIOA,GPIO_Pin_11);
//			}
//			else if(~command & 0b0000000010000000){
//				inpcommandfull = inpcommandfull ^ command;
//				inpcommandfull = inpcommandfull ^ 0b0000000010000000;
//				commandfull = inpcommandfull;
//				numoferror = 0;
//			}
//			else{
//				commandfull = precommandfull;
//				inpcommandfull = commandfull;
//				numoferror++;
//			}
//			if(numoferror >= 100){
//				while(1)GPIO_SetBits(GPIOA,GPIO_Pin_11);
//				commandfull = 0b1111111111111111;	//強制停止
//			}
//			USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
//	//		if(commandfull=0b1000001010000010)GPIO_SetBits(GPIOA,GPIO_Pin_11);
//	//		else GPIO_ResetBits(GPIOA,GPIO_Pin_11);
//	//		while(1);
//		}
//
//
//	}

void errorLED_composed( void){
	while(1){
			GPIO_SetBits(GPIOA,GPIO_Pin_11);
			tdelay(900);
			GPIO_ResetBits(GPIOA,GPIO_Pin_11);
			tdelay(100);
		}
}
