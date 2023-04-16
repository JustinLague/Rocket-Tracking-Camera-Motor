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
void update_motor_x(uint32_t speed);
void update_motor_y(uint32_t speed);
void update_motor_zoom(uint32_t speed);
void change_timer_def_for_freq(uint32_t freq, Timer_def_t* timer_def);
void change_speed(Timer_def_t* timer_def, uint32_t speed);


#endif /* APP_TSK_MOTOR_CONTROL_H_ */
