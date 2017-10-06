#include "stm32f4xx.h"
#include "make_motion.h"
#include "initialsetting.h"
#include "motion_define.h"
#include "usart3.h"

uint8_t sendbuf[10000];
uint8_t numofbuf;
uint8_t motionphase = 0;
extern uint32_t period;
extern uint32_t maxperiod;

void do_motion(uint16_t commandfull){
//	USART_Cmd(USART3,DISABLE);
	select_motion(commandfull);
}

void select_motion(uint16_t commandfull){
	//0b abcd efgh ijkl mnop
	//abcd upper
	//l Loop
	//k End
	//***when Start is nothing(but exist Loop), l = 1
		switch(commandfull){
		case adr_Neutral:
			GPIO_SetBits(GPIOA,GPIO_Pin_11);
			motionphase = 0;
			data_to_motion( (int16_t*)Neutral);
								break;



		case adr_Righthand:
			motionphase = 0;
			data_to_motion( (int16_t*)Righthand);
								break;
		case adr_Sit:
			motionphase = 0;
			data_to_motion( (int16_t*)Sit);
								break;

		case adr_torque_on:
			GPIO_ResetBits(GPIOA,GPIO_Pin_11);
			motionphase = 0;
			torque_on( (int16_t*)Neutral);
								break;
		case adr_torque_off:
			GPIO_ResetBits(GPIOA,GPIO_Pin_11);
			motionphase = 0;
//			data_to_motion( (int16_t*)Sit);
			torque_off( (int16_t*)Walk_front_Start);
//			tdelay(1000);
								break;
		case adr_Walk_front_Start:
		case adr_Attack_3_front_left:
		case adr_Attack_3_front_right:
		case adr_Walk_front_Start|0b0000100000000000:
			precommandfull = adr_Walk_front_Start;
			motionphase = 1;
			data_to_motion( (int16_t*)Walk_front_Start);
								break;
		case adr_Walk_front_Start|0b0000000000010000:
			motionphase = 2;
			data_to_motion( (int16_t*)Walk_front_Loop);
								break;
		case adr_Walk_front_Start|0b0000000000100000:
			motionphase = 3;
			data_to_motion( (int16_t*)Walk_front_End);
								break;
		case adr_Walk_front_from_left:
		case adr_Walk_front_from_left2:
			motionphase = 4;
			data_to_motion( (int16_t*)Walk_front_from_left);
								break;
		case adr_Walk_front_from_right:
		case adr_Walk_front_from_right2:
			motionphase = 4;
			data_to_motion( (int16_t*)Walk_front_from_right);
								break;
		case adr_Walk_left_Loop:
		case adr_Walk_left_Loop|0b0000000000010000:
		case adr_Walk_left_Loop|0b0000100000000000:
			precommandfull = adr_Walk_left_Loop;
			motionphase = 2;
			data_to_motion( (int16_t*)Walk_left_Loop);
								break;
		case adr_Walk_left_Loop|0b0000000000100000:
			motionphase = 3;
			data_to_motion( (int16_t*)Walk_left_End);
								break;
		case adr_Walk_left_from_front:
		case adr_Walk_left_from_front2:
			motionphase = 4;
			data_to_motion( (int16_t*)Walk_left_from_front);
								break;
		case adr_Walk_right_Loop:
		case adr_Walk_right_Loop|0b0000000000010000:
		case adr_Walk_right_Loop|0b0000100000000000:
			precommandfull = adr_Walk_right_Loop;
			motionphase = 2;
			data_to_motion( (int16_t*)Walk_right_Loop);
								break;
		case adr_Walk_right_Loop|0b0000000000100000:
			motionphase = 3;
			data_to_motion( (int16_t*)Walk_right_End);
								break;
		case adr_Walk_right_from_front:
		case adr_Walk_right_from_front2:
			motionphase = 4;
			data_to_motion( (int16_t*)Walk_right_from_front);
								break;


		case adr_Walk_front_righthand_Start:
			motionphase = 1;
			data_to_motion( (int16_t*)Walk_front_righthand_Start);
								break;
		case adr_Walk_front_righthand_Start|0b0000000000010000:
			motionphase = 2;
			data_to_motion( (int16_t*)Walk_front_righthand_Loop);
								break;
		case adr_Walk_front_righthand_Start|0b0000000000100000:
			motionphase = 3;
			data_to_motion( (int16_t*)Walk_front_righthand_End);
								break;
		case adr_Walk_front_from_left_righthand:
		case adr_Walk_front_from_left_righthand2:
			motionphase = 4;
			data_to_motion( (int16_t*)Walk_front_from_left_righthand);
								break;
		case adr_Walk_front_from_right_righthand:
		case adr_Walk_front_from_right_righthand2:
			motionphase = 4;
			data_to_motion( (int16_t*)Walk_front_from_right_righthand);
								break;
		case adr_Walk_left_righthand_Loop:
		case adr_Walk_left_righthand_Loop|0b0000000000010000:
			motionphase = 2;
			data_to_motion( (int16_t*)Walk_left_righthand_Loop);
								break;
		case adr_Walk_left_righthand_Loop|0b0000000000100000:
			motionphase = 3;
			data_to_motion( (int16_t*)Walk_left_righthand_End);
								break;
		case adr_Walk_left_from_front_righthand:
		case adr_Walk_left_from_front_righthand2:
			motionphase = 4;
			data_to_motion( (int16_t*)Walk_left_from_front_righthand);
								break;
		case adr_Walk_right_righthand_Loop:
		case adr_Walk_right_righthand_Loop|0b0000000000010000:
			motionphase = 2;
			data_to_motion( (int16_t*)Walk_right_righthand_Loop);
								break;
		case adr_Walk_right_righthand_Loop|0b0000000000100000:
			motionphase = 3;
			data_to_motion( (int16_t*)Walk_right_righthand_End);
								break;
		case adr_Walk_right_from_front_righthand:
		case adr_Walk_right_from_front_righthand2:
			motionphase = 4;
			data_to_motion( (int16_t*)Walk_right_from_front_righthand);
								break;


		case adr_Attack_1_ing_left|0b0011000000000000:
			motionphase = 0;
			data_to_motion( (int16_t*)Attack_1_ing_left);
								break;
		case adr_Attack_1_ing_right|0b0011000000000000:
			motionphase = 0;
			data_to_motion( (int16_t*)Attack_1_ing_right);
								break;
		case adr_Attack_2_sokkou_left:
			motionphase = 0;
			data_to_motion( (int16_t*)Attack_2_sokkou_left);
								break;
		case adr_Attack_2_sokkou_right:
			motionphase = 0;
			data_to_motion( (int16_t*)Attack_2_sokkou_right);
								break;
		case adr_Attack_3_front_left|0b0111000000000000:
			motionphase = 0;
			data_to_motion( (int16_t*)Attack_3_front_left);
								break;
		case adr_Attack_3_front_right|0b0111000000000000:
			motionphase = 0;
			data_to_motion( (int16_t*)Attack_3_front_right);
								break;
		case adr_Attack_5_simple:
			motionphase = 0;
			data_to_motion( (int16_t*)Attack_5_simple);
								break;
		case adr_Attack_6_righthand|0b0011000000000000:
			motionphase = 0;
			data_to_motion( (int16_t*)Attack_6_righthand);
								break;
		case adr_Attack_8_sutemi|0b0011000000000000:
			motionphase = 0;
			data_to_motion( (int16_t*)Attack_6_righthand);
								break;


		case adr_Stand_J:
			motionphase = 0;
			data_to_motion( (int16_t*)Stand_J);
								break;
		case adr_Stand_K:
			motionphase = 0;
			data_to_motion( (int16_t*)Stand_K);
								break;
		case adr_Banzai:
			motionphase = 0;
			data_to_motion( (int16_t*)Banzai);
								break;
		case adr_Tehuri_Start:
			motionphase = 0;
			data_to_motion( (int16_t*)Tehuri_Start);
								break;
		case adr_Tehuri_Loop:
			motionphase = 0;
			data_to_motion( (int16_t*)Tehuri_Loop);
								break;
		case adr_Juushin_Start:
			motionphase = 1;
			data_to_motion( (int16_t*)Juushin_Start);
								break;
		case adr_Juushin_Loop:
			motionphase = 2;
			data_to_motion( (int16_t*)Juushin_Loop);
								break;

		case adr_Turn_left_Loop:
		case adr_Turn_left_Loop|0b0000000000010000:
			motionphase = 2;
			data_to_motion( (int16_t*)Turn_left_Loop);
								break;
		case adr_Turn_right_Loop:
		case adr_Turn_right_Loop|0b0000000000010000:
			motionphase = 2;
			data_to_motion( (int16_t*)Turn_right_Loop);
								break;
		default:
//			GPIO_ResetBits(GPIOA,GPIO_Pin_11);
			motionphase = 0;
			data_to_motion( (int16_t*)Neutral);
								break;
	}
//	USART_Cmd(USART3,ENABLE);
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
	period = 0;
	maxperiod = 10000000;
//	precommandfull = adr_Neutral;
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
	period = 0;
	maxperiod = 10000000;
//	precommandfull = adr_Neutral;
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
