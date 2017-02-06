#ifndef MAKE_MOTION_H_
#define MAKE_MOTION_H_

extern uint8_t motionphase;

void do_motion(uint16_t commandfull);
void select_motion(uint16_t commandfull);
void torque_on(int16_t *motion);
void torque_off(int16_t *motion);
void data_to_motion(int16_t *motion);
void send_data(int16_t *motion);
void errorLED_command();

#endif
