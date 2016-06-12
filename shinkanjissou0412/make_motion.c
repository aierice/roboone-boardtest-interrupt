﻿#include "stm32f4xx.h"
#include "make_motion.h"
//#include "motion_data.h"
#include "initialsetting.h"
//#include "test_motion.c"

int16_t test_Start[3][15]={
{  0, 13,    1,    2,    3,    4,    5,    6,    7,    8,    9,   10,   11,   12,   13},//(一個目モーションの総フレーム数、二個目サーボの総数、三個目以降が指定したID)
{  0, 10,   -500,  300,  -50,  -50,   50,   50,  150, -300,  -50, -1500,  -50,  -50,  100},//初期姿勢(一個目移動時間、二個目待機時間、三個目以降角度)
{  0, 0,   500,    0,    0,    0,    0,    0,   50,  100,    0,-1000,  -50,  -50,  -50}//以下モーションデータ(一個目移動時間、二個目待機時間、三個目以降角度)
};

int16_t test_End[4][15]={
{  2, 13,    1,    2,    3,    4,    5,    6,    7,    8,    9,   10,   11,   12,   13},//(一個目モーションの総フレーム数、二個目サーボの総数、三個目以降が指定したID)
{  100, 10,    0,  300,  -50,  -50,   50,   50,  150, -300,  -50,-1500,  -50,  -50,  100},//初期姿勢(一個目移動時間、二個目待機時間、三個目以降角度)
{  20, 100, -150,    0,    0,    0,    0,    0,   50,  100,    0,    0,  -50,  -50,  -50},//以下モーションデータ(一個目移動時間、二個目待機時間、三個目以降角度)
{  50,   1, -250,  100,  100,  100,    0,    0,   50,   80,  150,  500,   50,   50,  -80}
};

extern uint8_t sendbuf[10000];
extern uint8_t numofbuf;
extern uint32_t period;
extern uint32_t maxperiod;
uint8_t DMA2flag = 0;

void do_motion(uint16_t commandfull){
	DMA2flag = 1;
	USART_Cmd(USART3,DISABLE);
	select_motion(commandfull);
}

void select_motion(uint16_t commandfull){
		switch(commandfull){
		case 0b1000000110000001:
			data_to_motion( (int16_t*)test_Start);
								break;
		case 0b1000001000000010:
			data_to_motion( (int16_t*)test_End);
								break;
		case 0b0111111111111111:
			torque_on( (int16_t*)test_Start);
								break;
		default:				errorLED_command();
	}
	USART_Cmd(USART3,ENABLE);
}

void torque_on( int16_t *motion){	//motion[i]*2を2*motion[i]に変え、uint8_tをint8_tにしたら動いた。//と思ったら動いていない//動いた。test_motionでIDが小さい方からすべて使われることが前提になっているので、注意
	sendbuf[0]=( int8_t) 0xFA;	//Header
	sendbuf[1]=( int8_t) 0xAF;
	sendbuf[2]=( int8_t) 0x00;	//ID 	:keep 0
	sendbuf[3]=( int8_t) 0x00;	//Flag 	:keep 0
	sendbuf[4]=( int8_t) 0x24;	//Address num of torqueon
	sendbuf[5]=( int8_t) 0x02;	//Length
	sendbuf[6]=( int8_t) motion[1];	//num of motion
	int i;
	for( i = 1; i <= motion[1];i++){
		sendbuf[5+i*2] = ( int8_t) i;	//VID
		sendbuf[6+i*2] = ( int8_t) 0x01;	//torqueon
	}
	uint8_t checksum = 0;
	for( i = 2; i <=6+2*motion[1]; i++){
		checksum = checksum ^ sendbuf[i];
	}
	sendbuf[7+2*motion[1]] = checksum;
	numofbuf = 7+2*motion[1];
	send_data( (int16_t*)motion);
}

void data_to_motion( int16_t *motion){
	sendbuf[0] = ( int8_t) 0xFA;	//ロングパケットでは固定
	sendbuf[1] = ( int8_t) 0xAF;	//ロングパケットでは固定
	sendbuf[2] = ( int8_t) 0x00;	//ロングパケットでは固定
	sendbuf[3] = ( int8_t) 0x00;	//ロングパケットでは固定
	sendbuf[4] = ( int8_t) 0x1E;	//ロングパケットでは固定
	sendbuf[5] = ( int8_t) 0x05;	//各要素の数の合計。VID(ID)+位置指定+時間指定=5
	sendbuf[6] = ( int8_t) motion[1];	//モーターの数
	static	uint8_t j;
	uint16_t i = 0;
	static uint8_t k;
	int8_t checksum = 0;
	if(j < 1+motion[0]){
		if(k == 0){
//			sendbuf[7] = ( int8_t) 1;
//			sendbuf[8] = ( int8_t) 0x11;
//			sendbuf[9] = ( int8_t) 0x22;
//			sendbuf[10]= ( int8_t) 0x64;
//			sendbuf[11]= ( int8_t) 0x00;
			for( i = 0; i < motion[1]; i++){	//移動
				sendbuf[ 7+i*5] = ( int8_t) motion[i+2];	//IDの指定
				sendbuf[ 8+i*5] = ( int8_t) (( int16_t)motion[(j+1)*( 2+motion[1])+(i+2)] &0xFF);	//位置
				sendbuf[ 9+i*5] = ( int8_t) (( int16_t)motion[(j+1)*( 2+motion[1])+(i+2)] >>8);
				sendbuf[10+i*5] = ( int8_t) (( int16_t)motion[(j+1)*( 2+motion[1])] &0xFF);
				sendbuf[11+i*5] = ( int8_t) (( int16_t)motion[(j+1)*( 2+motion[1])] >>8);	//時間
			}
			checksum = 0;
			for( i = 2; i <= 6+5*motion[1]; i++){
					checksum = checksum ^ sendbuf[i];
			}
			sendbuf[ 7+5*motion[1]] = ( int8_t)checksum;
			numofbuf = 7+5* motion[1];
			send_data( (int16_t*)motion);
			period = 0;
			maxperiod = motion[ ( j+1)*( 2+motion[1])];		//maxperiod = 10*motion[ ( j+1)*( 2+motion[1])];
			k++;
		}
		else{
			for(i = 0; i < motion[1]; i++){	//待機
				sendbuf[ 7+i*5] = ( int8_t) motion[i+2];	//IDの指定
				sendbuf[ 8+i*5] = ( int8_t) (( int16_t)motion[(j+1)*( 2+motion[1])+(i+2)] &0xFF);	//位置
				sendbuf[ 9+i*5] = ( int8_t) (( int16_t)motion[(j+1)*( 2+motion[1])+(i+2)] >>8);
				sendbuf[10+i*5] = ( int8_t) (( int16_t)motion[(j+1)*( 2+motion[1])+1] &0xFF);
				sendbuf[11+i*5] = ( int8_t) (( int16_t)motion[(j+1)*( 2+motion[1])+1] >>8);	//時間
			}
			checksum = 0;
			for( i = 2; i <= 6+5*motion[1]; i++){
				checksum = checksum ^ sendbuf[i];
			}
			sendbuf[ 7+5*motion[1]] = ( int8_t)checksum;
			numofbuf = 7+5*motion[1];
			send_data( (int16_t*)motion);
			j++;
			k = 0;
			period = 0;
			maxperiod = motion[ ( j+1)*( 2+motion[1])+1];		//maxperiod = 10*motion[ ( j+1)*( 2+motion[1])+1];
		}
	}
	else{
		j = 0;
		maxperiod = 10000000;
	}
}

void send_data(int16_t *motion){
//	uint8_t counter = 0;
//	while( numofbuf >= counter){
//		while( USART_GetFlagStatus( USART1,USART_FLAG_TXE)==RESET);
//		USART_SendData( USART1,sendbuf[counter]);
//		counter++;
//	}
	DMA_SetCurrDataCounter(DMA2_Stream7, numofbuf+1);
	DMA_Cmd(DMA2_Stream7, ENABLE);
}

void errorLED_command(){
	while(1){
		GPIO_SetBits(GPIOA,GPIO_Pin_11);
		tdelay(500);
		GPIO_ResetBits(GPIOA,GPIO_Pin_11);
		tdelay(500);
	}
}

/*もしかするといらない*/
void DMA2_Stream7_IRQHandler(void){
	if(DMA_GetITStatus(DMA2_Stream7,DMA_IT_TCIF7)){
//		DMA_Cmd(DMA2_Stream7, DISABLE);							// TX DMA Stop
//		ServoCommandData.TX_RX_isRunning = 0;					// TX RX stop
		DMA2flag = 0;
		DMA_ClearITPendingBit(DMA2_Stream7,DMA_IT_TCIF7);		// clear interrupt flag
//		DMA_Cmd(DMA2_Stream7, DISABLE);
	}
}
