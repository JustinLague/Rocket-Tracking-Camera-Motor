/*
 * motor.h
 *
 *  Created on: Mar 31, 2023
 *      Author: Justin
 */

#ifndef APP_MOTOR_H_
#define APP_MOTOR_H_

/******************************************************************************/
/*                             Includes	                                      */
/******************************************************************************/

/******************************************************************************/
/*                             Typedef prototype                              */
/******************************************************************************/
typedef struct {
   uint32_t arr;
   uint32_t psc;
} Timer_def_t;

/******************************************************************************/
/*                             Function prototype                             */
/******************************************************************************/
void tsk_motor_control();
void update_motor_x();
void update_motor_y();
void update_motor_zoom();
void change_timer_def_for_freq(uint32_t freq, Timer_def_t* timer_def);
void change_speed(Timer_def_t* timer_def, uint32_t speed);


#endif /* APP_MOTOR_H_ */
