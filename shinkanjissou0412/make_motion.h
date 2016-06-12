#ifndef MAKE_MOTION_H_
#define MAKE_MOTION_H_

uint8_t sendbuf[10000];
uint8_t numofbuf;

void do_motion(uint16_t commandfull);
void select_motion(uint16_t commandfull);
void torque_on(int16_t *motion);
void data_to_motion(int16_t *motion);
void send_data(int16_t *motion);
void errorLED_command();

#endif
