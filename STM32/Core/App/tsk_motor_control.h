/*
 * tsk_motor_control.h
 *
 *  Created on: Mar 31, 2023
 *      Author: Justin
 */

#ifndef APP_TSK_MOTOR_CONTROL_H_
#define APP_TSK_MOTOR_CONTROL_H_

/******************************************************************************/
/*                             Includes	                                      */
/******************************************************************************/
#include "tsk_i2c.h"
/******************************************************************************/
/*                             Typedef prototype                              */
/******************************************************************************/


/******************************************************************************/
/*                             Function prototype                             */
/******************************************************************************/
void tsk_motor_control();
void start_motor_pwm(enum motor_e motor);
void stop_motor_pwm(enum motor_e motor);
void update_motor(enum motor_e motor);
void update_mode(uint8_t mode, enum motor_e motor);
void update_dir(uint8_t dir, enum motor_e motor);
void update_pwm(motor_t* motor);
void update_timer(uint32_t pwm_frequency, Timer_def_t* timer);


#endif /* APP_TSK_MOTOR_CONTROL_H_ */
