/*
 * tsk_i2c.c
 *
 *  Created on: Apr 5, 2023
 *      Author: Justin
 */


/******************************************************************************/
/*                             Includes	                                      */
/******************************************************************************/
#include "main.h"
#include "tsk_i2c.h"
#include <string.h>


/******************************************************************************/
/*                             Global variable                                */
/******************************************************************************/
motor_t motor_x;
motor_t motor_y;
motor_t motor_zoom;

osMutexId_t update_motor_mutexHandle;
const osMutexAttr_t update_motor_mutex_attributes = {
  .name = "UPDATE_speed_mutex"
};

uint8_t aRxBuffer[7];
uint8_t aTxBuffer[5];

#define RXBUFFERSIZE                      (COUNTOF(aRxBuffer))
#define TXBUFFERSIZE                      (COUNTOF(aTxBuffer))
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

#define UPDATE_SPEED_MOTORS 0x10
#define UPDATE_SPEED_MOTOR_X 0x11
#define UPDATE_SPEED_MOTOR_Y 0x12
#define UPDATE_SPEED_MOTOR_ZOOM 0x13
#define UPDATE_MAX_ACCELERATION_MOTORS 0x20
#define UPDATE_MAX_ACCELERATION_X 0x21
#define UPDATE_MAX_ACCELERATION_Y 0x22
#define UPDATE_MAX_ACCELERATION_ZOOM 0x23
#define UPDATE_DIRECTION_MOTORS 0x30
#define UPDATE_DIRECTION_X 0x31
#define UPDATE_DIRECTION_Y 0x32
#define UPDATE_DIRECTION_ZOOM 0x33
#define UPDATE_MODE_MOTORS 0x40
#define UPDATE_MODE_X 0x41
#define UPDATE_MODE_Y 0x42
#define UPDATE_MODE_ZOOM 0x43



uint8_t uwTransferRequested = 0;

uint8_t debug_buf[12];

/******************************************************************************/
/*                             Function implementation                        */
/******************************************************************************/

void tsk_i2c() {
	update_motor_mutexHandle = osMutexNew(&update_motor_mutex_attributes);

	init_motor(&motor_x);
	motor_x.type = MOTOR_X;

	init_motor(&motor_y);
	motor_y.type = MOTOR_Y;

	init_motor(&motor_zoom);
	motor_zoom.type = MOTOR_ZOOM;

	if(HAL_I2C_EnableListen_IT(get_hi2cl()) != HAL_OK) {
		Error_Handler();
	}

	while(1){
		osDelay(10000);
	}
}



void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *I2cHandle) {
	uint8_t reg = aRxBuffer[0];

	//UPDATE speed of motors
	if(reg == UPDATE_SPEED_MOTORS) {
		osMutexAcquire(update_motor_mutexHandle, 0);
			motor_x.desired_speed.uint8Val[0] = aRxBuffer[1];
			motor_x.desired_speed.uint8Val[1] = aRxBuffer[2];
			motor_y.desired_speed.uint8Val[0] = aRxBuffer[3];
			motor_y.desired_speed.uint8Val[1] = aRxBuffer[4];
			motor_zoom.desired_speed.uint8Val[0] = aRxBuffer[5];
			motor_zoom.desired_speed.uint8Val[1] = aRxBuffer[6];
		osMutexRelease(update_motor_mutexHandle);
	}

	//UPDATE speed of motor x
	if(reg == UPDATE_SPEED_MOTOR_X) {
		osMutexAcquire(update_motor_mutexHandle, 0);
			motor_x.desired_speed.uint8Val[0] = aRxBuffer[1];
			motor_x.desired_speed.uint8Val[1] = aRxBuffer[2];
		osMutexRelease(update_motor_mutexHandle);
	}

	//UPDATE speed of motor y
	if(reg == UPDATE_SPEED_MOTOR_Y) {
		osMutexAcquire(update_motor_mutexHandle, 0);
			motor_y.desired_speed.uint8Val[0] = aRxBuffer[1];
			motor_y.desired_speed.uint8Val[1] = aRxBuffer[2];
		osMutexRelease(update_motor_mutexHandle);
	}

	//UPDATE speed of motor zoom
	if(reg == UPDATE_SPEED_MOTOR_ZOOM) {
		osMutexAcquire(update_motor_mutexHandle, 0);
			motor_zoom.desired_speed.uint8Val[0] = aRxBuffer[1];
			motor_zoom.desired_speed.uint8Val[1] = aRxBuffer[2];
		osMutexRelease(update_motor_mutexHandle);
	}

	if(reg == UPDATE_MAX_ACCELERATION_MOTORS) {
		osMutexAcquire(update_motor_mutexHandle, 0);
			motor_x.max_acceleration.uint8Val[0] = aRxBuffer[1];
			motor_x.max_acceleration.uint8Val[1] = aRxBuffer[2];
			motor_y.max_acceleration.uint8Val[0] = aRxBuffer[3];
			motor_y.max_acceleration.uint8Val[1] = aRxBuffer[4];
			motor_zoom.max_acceleration.uint8Val[0] = aRxBuffer[5];
			motor_zoom.max_acceleration.uint8Val[1] = aRxBuffer[6];
		osMutexRelease(update_motor_mutexHandle);
	}

	if(reg == UPDATE_MAX_ACCELERATION_X) {
		osMutexAcquire(update_motor_mutexHandle, 0);
			motor_x.max_acceleration.uint8Val[0] = aRxBuffer[1];
			motor_x.max_acceleration.uint8Val[1] = aRxBuffer[2];
		osMutexRelease(update_motor_mutexHandle);
	}

	if(reg == UPDATE_MAX_ACCELERATION_Y) {
		osMutexAcquire(update_motor_mutexHandle, 0);
			motor_y.max_acceleration.uint8Val[0] = aRxBuffer[1];
			motor_y.max_acceleration.uint8Val[1] = aRxBuffer[2];
		osMutexRelease(update_motor_mutexHandle);
	}

	if(reg == UPDATE_MAX_ACCELERATION_ZOOM) {
		osMutexAcquire(update_motor_mutexHandle, 0);
			motor_zoom.max_acceleration.uint8Val[0] = aRxBuffer[1];
			motor_zoom.max_acceleration.uint8Val[1] = aRxBuffer[2];
		osMutexRelease(update_motor_mutexHandle);
	}

	//UPDATE direction of motors
	if(reg == UPDATE_DIRECTION_MOTORS) {
		osMutexAcquire(update_motor_mutexHandle, 0);
			motor_x.dir = aRxBuffer[1];
			motor_y.dir = aRxBuffer[2];
			motor_zoom.dir = aRxBuffer[3];
		osMutexRelease(update_motor_mutexHandle);
	}

	//UPDATE direction of motor x
	if(reg == UPDATE_DIRECTION_X) {
		osMutexAcquire(update_motor_mutexHandle, 0);
			motor_x.dir = aRxBuffer[1];
		osMutexRelease(update_motor_mutexHandle);
	}

	//UPDATE direction of motor y
	if(reg == UPDATE_DIRECTION_Y) {
		osMutexAcquire(update_motor_mutexHandle, 0);
			motor_y.dir = aRxBuffer[1];
		osMutexRelease(update_motor_mutexHandle);
	}

	//UPDATE direction of motor zoom
	if(reg == UPDATE_DIRECTION_ZOOM) {
		osMutexAcquire(update_motor_mutexHandle, 0);
			motor_zoom.dir = aRxBuffer[1];
		osMutexRelease(update_motor_mutexHandle);
	}

	//UPDATE mode of motors
	if(reg == UPDATE_MODE_MOTORS) {
		osMutexAcquire(update_motor_mutexHandle, 0);
			motor_x.mode = aRxBuffer[1];
			motor_y.mode = aRxBuffer[2];
			motor_zoom.mode = aRxBuffer[3];
		osMutexRelease(update_motor_mutexHandle);
	}

	//UPDATE mode of motor x
	if(reg == UPDATE_MODE_X) {
		osMutexAcquire(update_motor_mutexHandle, 0);
			motor_x.mode = aRxBuffer[1];
		osMutexRelease(update_motor_mutexHandle);
	}

	//UPDATE mode of motor y
	if(reg == UPDATE_MODE_Y) {
		osMutexAcquire(update_motor_mutexHandle, 0);
			motor_y.mode = aRxBuffer[1];
		osMutexRelease(update_motor_mutexHandle);
	}

	//UPDATE mode of motor zoom
	if(reg == UPDATE_MODE_ZOOM) {
		osMutexAcquire(update_motor_mutexHandle, 0);
			motor_zoom.mode = aRxBuffer[1];
		osMutexRelease(update_motor_mutexHandle);
	}
}

void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
	if (TransferDirection != 0) {
		 /*##- Start the transmission process #####################################*/
		/* While the I2C in reception process, user can transmit data through
		 "aTxBuffer" buffer */
		if (HAL_I2C_Slave_Seq_Transmit_IT(get_hi2cl(), (uint8_t *)aTxBuffer, TXBUFFERSIZE, I2C_NEXT_FRAME) != HAL_OK) {
			/* Transfer error in transmission process */
			Error_Handler();
		}
	} else {
		/*##- Put I2C peripheral in reception process ###########################*/
		if (HAL_I2C_Slave_Seq_Receive_IT(get_hi2cl(), (uint8_t *)aRxBuffer, 1, I2C_NEXT_FRAME) != HAL_OK) {
			/* Transfer error in reception process */
			Error_Handler();
		}
		if (HAL_I2C_Slave_Seq_Receive_IT(get_hi2cl(), (uint8_t *)aTxBuffer, 1, I2C_NEXT_FRAME) != HAL_OK) {
			/* Transfer error in reception process */
			Error_Handler();
		}
	}
}

void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{
	//strcpy((char*) debug_buf, "ListenCplt\r\n");
	//HAL_UART_Transmit(get_huart2(), debug_buf, strlen((char*)debug_buf), HAL_MAX_DELAY);

	if(HAL_I2C_EnableListen_IT(get_hi2cl()) != HAL_OK) {
		Error_Handler();
	}
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *I2cHandle)
{
  /** Error_Handler() function is called when error occurs.
    * 1- When Slave doesn't acknowledge its address, Master restarts communication.
    * 2- When Master doesn't acknowledge the last data transferred, Slave doesn't care in this example.
    */

	strcpy((char*) debug_buf, "Erreur\r\n");
	HAL_UART_Transmit(get_huart2(), debug_buf, strlen((char*)debug_buf), HAL_MAX_DELAY);

	if (HAL_I2C_GetError(I2cHandle) != HAL_I2C_ERROR_AF) {
		strcpy((char*) debug_buf, "Erreur AF\r\n");
		HAL_UART_Transmit(get_huart2(), debug_buf, strlen((char*)debug_buf), HAL_MAX_DELAY);
		Error_Handler();
	}
}

void init_motor(motor_t* motor) {
	motor->dir = 0;
	motor->mode = 0;
	motor->desired_speed.uint16Val = 0;
	motor->max_acceleration.uint16Val = 2;
	motor->actual_speed = 0;
}

motor_t* get_motor(enum motor_e _motor) {
	motor_t* motor;

	switch(_motor){
			case MOTOR_X:
				osMutexAcquire(update_motor_mutexHandle, 0);
					motor = &motor_x;
				osMutexRelease(update_motor_mutexHandle);
				break;
			case MOTOR_Y:
				osMutexAcquire(update_motor_mutexHandle, 0);
					motor = &motor_y;
				osMutexRelease(update_motor_mutexHandle);
				break;
			case MOTOR_ZOOM:
				osMutexAcquire(update_motor_mutexHandle, 0);
					motor = &motor_zoom;
				osMutexRelease(update_motor_mutexHandle);
				break;
		}

	return motor;
}
