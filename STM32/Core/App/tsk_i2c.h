/*
 * i2c_task.h
 *
 *  Created on: Apr 5, 2023
 *      Author: Justin
 */

#ifndef APP_TSK_I2C_H_
#define APP_TSK_I2C_H_

/******************************************************************************/
/*                             Includes	                                      */
/******************************************************************************/

/******************************************************************************/
/*                             Typedef prototype                              */
/******************************************************************************/
union uint16_u {
	uint8_t uint8Val[2];
    uint16_t uint16Val;
};

typedef struct {
   uint32_t arr;
   uint32_t psc;
} Timer_def_t;

typedef struct {
	enum motor_e type;
	union uint16_u desired_speed;
	union uint16_u max_acceleration;
	uint16_t actual_speed;
	uint8_t dir;
	uint8_t mode;
	Timer_def_t timer;
} motor_t;


/******************************************************************************/
/*                             Function prototype                             */
/******************************************************************************/
void tsk_i2c();
void init_motor(motor_t* motor);
motor_t* get_motor(enum motor_e _motor);

#endif /* APP_TSK_I2C_H_ */
