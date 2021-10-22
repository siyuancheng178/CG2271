#ifndef MOTOR_H
#define MOTOR_H
void moving_forward();
void moving_backward();
void moving_left();
void moving_right();
void stop();
void change_duty_cycle(int new_duty_cycle);
void initMotor();


#endif
