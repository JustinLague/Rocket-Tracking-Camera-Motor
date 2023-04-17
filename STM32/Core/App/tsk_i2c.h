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
	union uint16_u speed_x;
	union uint16_u speed_y;
	union uint16_u speed_zoom;
} motor_speed_t;


/******************************************************************************/
/*                             Function prototype                             */
/******************************************************************************/
void tsk_i2c();
uint16_t get_speed_x();
uint16_t get_speed_y();
uint16_t get_speed_zoom();


#endif /* APP_TSK_I2C_H_ */
