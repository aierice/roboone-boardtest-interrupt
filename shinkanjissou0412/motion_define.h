#include "stm32f4xx.h"

#ifndef MOTION_DEFINE_H_
#define MOTION_DEFINE_H_
#define adr_test	0b1100000000000000
#define adr_Neutral	0b1000000000000000
#define adr_torque_on	0b1000100000000000
#define adr_torque_off	0b1000000100000000
#define adr_Walk_front	0b1000000000000001
#define adr_Walk_behind	0b1000000000000010
#define adr_Walk_left	0b1000000000000100
#define adr_Walk_right	0b1000000000001000
#define adr_Atk_left	0b1000001000001000
#define adr_Atk_right	0b1000001000000100
#define adr_Stand_front	0b1000001000000001
#define adr_Stand_behind	0b1000001000000010
#define adr_Banzai	0b1000000000000011
#define adr_Tehuri	0b1000000000001100

extern int16_t test_Start[3][3];
extern int16_t test_End[3][3];
extern int16_t test_Test[3][3];
extern int16_t Neutral[3][21];
extern int16_t Templete[3][21];
extern int16_t Walk_front_Start[3][21];
extern int16_t Walk_front_Loop[10][21];
extern int16_t Walk_front_End[4][21];
extern int16_t Walk_behind_Loop[8][21];
extern int16_t Walk_behind_End[5][21];
extern int16_t Walk_left_Loop[6][21];
extern int16_t Walk_left_End[5][21];
extern int16_t Walk_left_Loop_from_front[6][21];
extern int16_t Walk_right_Loop[6][21];
extern int16_t Walk_right_End[5][21];
extern int16_t Walk_right_Loop_from_front[6][21];
extern int16_t Atk_left[8][21];
extern int16_t Atk_right[8][21];
extern int16_t Stand_front[9][21];
extern int16_t Stand_behind[8][21];
extern int16_t Banzai[6][21];
extern int16_t Tehuri[4][21];
#endif
