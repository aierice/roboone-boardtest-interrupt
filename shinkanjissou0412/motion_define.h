#include "stm32f4xx.h"

#ifndef MOTION_DEFINE_H_
#define MOTION_DEFINE_H_
#define adr_test	0b1000001100001111
#define adr_test2	0b1000001100000111
#define adr_Neutral	0b1000000000000000
#define adr_torque_on	0b1000100000000000
#define adr_torque_off	0b1000000100000000
#define adr_Walk_front	0b1000000000000001
#define adr_Walk_behind	0b1000000000000010
#define adr_Walk_left	0b1000000000000100
#define adr_Walk_right	0b1000000000001000
#define adr_Walk_left_front	0b1000000000000101
#define adr_Walk_right_front	0b1000000000001001
#define adr_Atk_3		0b1000010000000001
#define adr_Atk_left	0b1000001000001000
#define adr_Atk_right	0b1000001000000100
#define adr_Stand_front	0b1000001000000001
#define adr_Stand_behind	0b1000001000000010
#define adr_Banzai	0b1000000000001111
#define adr_Tehuri	0b1000000000001100

extern int16_t test_Start[3][3];
extern int16_t test_Loop[3][3];
extern int16_t test_End[3][3];
extern int16_t test2_from_test[3][3];
extern int16_t test2[3][3];
extern int16_t Neutral[3][21];
extern int16_t Templete[3][21];
extern int16_t Walk_front_Start[4][21];
extern int16_t Walk_front_Loop[8][21];
extern int16_t Walk_front_End[5][21];
extern int16_t Walk_front_from_right[6][21];
extern int16_t Walk_front_from_left[4][21];
extern int16_t Atk_3[14][21];
extern int16_t Atk_3_from_front[6][21];
extern int16_t Walk_behind_Loop[8][21];
extern int16_t Walk_behind_End[5][21];
extern int16_t Walk_left_Loop[6][21];
extern int16_t Walk_left_End[6][21];
extern int16_t Walk_left_from_front[6][21];
extern int16_t Walk_right_Loop[6][21];
extern int16_t Walk_right_End[6][21];
extern int16_t Walk_right_from_front[5][21];
extern int16_t Atk_left[8][21];
extern int16_t Atk_right[8][21];
extern int16_t Stand_front[9][21];
extern int16_t Stand_behind[8][21];
extern int16_t Banzai[6][21];
extern int16_t Tehuri[4][21];
#endif
