#include "stm32f4xx.h"
#include "make_motion.h"
#include "initialsetting.h"

extern int16_t test_Start[3][3];
extern int16_t test_End[3][3];
extern int16_t test_Test[3][3];
extern int16_t Templete[3][21];
extern int16_t Walk_behind_Loop[8][21];
extern int16_t Walk_behind_End[5][21];
extern int16_t Walk_front_Start[3][21];
extern int16_t Walk_front_Loop[10][21];
extern int16_t Walk_front_End[4][21];
extern int16_t Walk_left_Loop[6][21];
extern int16_t Walk_left_End[5][21];
extern int16_t Walk_right_Loop[6][21];
extern int16_t Walk_right_End[5][21];
extern int16_t Atk_left[8][21];
extern int16_t Atk_right[8][21];
extern int16_t Stand_front[9][21];
extern int16_t Stand_behind[8][21];
extern int16_t Neutral[3][21];
extern int16_t Banzai[6][21];
extern int16_t Tehuri[4][21];
extern int16_t Templete[3][21];
extern int16_t Neutral[3][21];

uint8_t sendbuf[10000];
uint8_t numofbuf;
uint8_t motionphase = 0;
extern uint32_t period;
extern uint32_t maxperiod;

void do_motion(uint16_t commandfull){
	USART_Cmd(USART3,DISABLE);
	select_motion(commandfull);
}

void select_motion(uint16_t commandfull){
	//0b abcd efgh ijkl mnop
	//abcd upper
	//l Loop
	//k End
	//***when Start is nothing(but exist Loop), l = 1
		switch(commandfull){
		case 0b1000000000000000:
			motionphase = 0;
			data_to_motion( (int16_t*)Neutral);
								break;
		case 0b1000100000000000:
			motionphase = 0;
			torque_on( (int16_t*)Templete);
								break;
		case 0b1000000100000000:
			motionphase = 0;
			torque_off( (int16_t*)Templete);
								break;
		case 0b1000000000000001:
			motionphase = 1;
			data_to_motion( (int16_t*)Walk_front_Start);
								break;
		case 0b1000000000010001:
			motionphase = 2;
			data_to_motion( (int16_t*)Walk_front_Loop);
								break;
		case 0b1000000000100001:
			motionphase = 3;
			data_to_motion( (int16_t*)Walk_front_End);
								break;
		case 0b1000000000000010:
			motionphase = 2;
			data_to_motion( (int16_t*)Walk_behind_Loop);
								break;
		case 0b1000000000100010:
			motionphase = 3;
			data_to_motion( (int16_t*)Walk_behind_End);
								break;
		case 0b1000000000000100:
			motionphase = 2;
			data_to_motion( (int16_t*)Walk_left_Loop);
								break;
		case 0b1000000000100100:
			motionphase = 3;
			data_to_motion( (int16_t*)Walk_left_End);
								break;
		case 0b1000000000001000:
			motionphase = 2;
			data_to_motion( (int16_t*)Walk_right_Loop);
								break;
		case 0b1000000000101000:
			motionphase = 3;
			data_to_motion( (int16_t*)Walk_right_End);
								break;
		case 0b1000001000001000:
			motionphase = 0;
			data_to_motion( (int16_t*)Atk_left);
								break;
		case 0b1000001000000100:
			motionphase = 0;
			data_to_motion( (int16_t*)Atk_right);
								break;
		case 0b1000001000000001:
			motionphase = 0;
			data_to_motion( (int16_t*)Stand_front);
								break;
		case 0b1000001000000010:
			motionphase = 0;
			data_to_motion( (int16_t*)Stand_behind);
								break;
		case 0b1000000000000011:
			motionphase = 0;
			data_to_motion( (int16_t*)Banzai);
								break;
		case 0b1000000000001100:
			motionphase = 0;
			data_to_motion( (int16_t*)Tehuri);
								break;
		case 0b1000100100000000:
			motionphase = 0;
			data_to_motion( (int16_t*)Templete);
		case 0b0111111111111111:
			motionphase = 0;
			torque_on( (int16_t*)Templete);
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
	sendbuf[4]=( int8_t) 0x24;	//Address num of torque on
	sendbuf[5]=( int8_t) 0x02;	//Length
	sendbuf[6]=( int8_t) motion[1];	//num of motion
	int i;
	for( i = 1; i <= motion[1];i++){
		sendbuf[5+i*2] = ( int8_t) i;	//VID
		sendbuf[6+i*2] = ( int8_t) 0x01;	//torque on
	}
	uint8_t checksum = 0;
	for( i = 2; i <=6+2*motion[1]; i++){
		checksum = checksum ^ sendbuf[i];
	}
	sendbuf[7+2*motion[1]] = checksum;
	numofbuf = 7+2*motion[1];
	send_data( (int16_t*)motion);
}

void torque_off( int16_t *motion){	//motion[i]*2を2*motion[i]に変え、uint8_tをint8_tにしたら動いた。//と思ったら動いていない//動いた。test_motionでIDが小さい方からすべて使われることが前提になっているので、注意
	sendbuf[0]=( int8_t) 0xFA;	//Header
	sendbuf[1]=( int8_t) 0xAF;
	sendbuf[2]=( int8_t) 0x00;	//ID 	:keep 0
	sendbuf[3]=( int8_t) 0x00;	//Flag 	:keep 0
	sendbuf[4]=( int8_t) 0x24;	//Address num of torque on
	sendbuf[5]=( int8_t) 0x02;	//Length
	sendbuf[6]=( int8_t) motion[1];	//num of motion
	int i;
	for( i = 1; i <= motion[1];i++){
		sendbuf[5+i*2] = ( int8_t) i;	//VID
		sendbuf[6+i*2] = ( int8_t) 0x00;	//torque off
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
	static	uint8_t motionorder = 1;
	uint16_t i = 0;
	static uint8_t moveorwait = 0;
	int8_t checksum = 0;
	if(motionorder < 1+motion[0]){
		if(moveorwait == 0){
			if(motion[(motionorder+1)*( 2+motion[1])] == 0){
				numofbuf = 7+5* motion[1];
				moveorwait = 1;
				period = 0;
				maxperiod = 0;
			}
			else{
				for( i = 0; i < motion[1]; i++){	//移動
					sendbuf[ 7+i*5] = ( int8_t) motion[i+2];	//IDの指定
					sendbuf[ 8+i*5] = ( int8_t) ((( int16_t)motion[(motionorder+1)*( 2+motion[1])+(i+2)]+( int16_t)motion[motion[1]+i+4])& 0xFF);	//位置
					sendbuf[ 9+i*5] = ( int8_t) ((( int16_t)motion[(motionorder+1)*( 2+motion[1])+(i+2)]+( int16_t)motion[motion[1]+i+4]) >>8);
					sendbuf[10+i*5] = ( int8_t) (( int16_t)motion[(motionorder+1)*( 2+motion[1])] &0xFF);
					sendbuf[11+i*5] = ( int8_t) (( int16_t)motion[(motionorder+1)*( 2+motion[1])] >>8);	//時間
				}
				checksum = 0;
				for( i = 2; i <= 6+5*motion[1]; i++){
					checksum = checksum ^ sendbuf[i];
				}
				sendbuf[ 7+5*motion[1]] = ( int8_t)checksum;
				numofbuf = 7+5* motion[1];
				send_data( (int16_t*)motion);
				moveorwait = 1;
				period = 0;
				maxperiod = 10*motion[ ( motionorder+1)*( 2+motion[1])];		//maxperiod = 10*motion[ ( j+1)*( 2+motion[1])];
				}
		}
		else{
			if(motion[(motionorder+1)*( 2+motion[1])+1] == 0){
				numofbuf = 7+5* motion[1];
				moveorwait = 0;
				period = 0;
				maxperiod = 0;
			}
			else{
				for(i = 0; i < motion[1]; i++){	//待機
					sendbuf[ 7+i*5] = ( int8_t) motion[i+2];	//IDの指定
					sendbuf[ 8+i*5] = ( int8_t) ((( int16_t)motion[(motionorder+1)*( 2+motion[1])+(i+2)]+( int16_t)motion[motion[1]+i+4]) &0xFF);	//位置
					sendbuf[ 9+i*5] = ( int8_t) ((( int16_t)motion[(motionorder+1)*( 2+motion[1])+(i+2)]+( int16_t)motion[motion[1]+i+4]) >>8);
					sendbuf[10+i*5] = ( int8_t) (( int16_t)motion[(motionorder+1)*( 2+motion[1])+1] &0xFF);
					sendbuf[11+i*5] = ( int8_t) (( int16_t)motion[(motionorder+1)*( 2+motion[1])+1] >>8);	//時間
				}
				checksum = 0;
				for( i = 2; i <= 6+5*motion[1]; i++){
					checksum = checksum ^ sendbuf[i];
				}
				sendbuf[ 7+5*motion[1]] = ( int8_t)checksum;
				numofbuf = 7+5*motion[1];
				send_data( (int16_t*)motion);
				moveorwait = 0;
				period = 0;
				maxperiod = 10*motion[ ( motionorder+1)*( 2+motion[1])+1];		//maxperiod = 10*motion[ ( j+1)*( 2+motion[1])+1];
			}
			motionorder++;
		}
	}
	else{
		motionorder = 1;
		maxperiod = 10000000;
	}
}

void send_data(int16_t *motion){
	DMA_SetCurrDataCounter(DMA2_Stream7, numofbuf+1);
	DMA_Cmd(DMA2_Stream7, ENABLE);	//none disable...OK?
}

void errorLED_command(){
}

/*もしかするといらない*/
void DMA2_Stream7_IRQHandler(void){
	if(DMA_GetITStatus(DMA2_Stream7,DMA_IT_TCIF7)){
//		DMA_Cmd(DMA2_Stream7, DISABLE);							// TX DMA Stop
//		ServoCommandData.TX_RX_isRunning = 0;					// TX RX stop
		DMA_ClearITPendingBit(DMA2_Stream7,DMA_IT_TCIF7);		// clear interrupt flag
//		DMA_Cmd(DMA2_Stream7, DISABLE);
	}
}
