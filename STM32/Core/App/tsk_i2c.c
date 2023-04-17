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
motor_speed_t motor_speed;

osMutexId_t Change_speed_mutexHandle;
const osMutexAttr_t Change_speed_mutex_attributes = {
  .name = "Change_speed_mutex"
};

uint8_t aRxBuffer[6];
uint8_t aTxBuffer[5];

#define RXBUFFERSIZE                      (COUNTOF(aRxBuffer))
#define TXBUFFERSIZE                      (COUNTOF(aTxBuffer))
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

uint8_t uwTransferRequested = 0;

uint8_t debug_buf[12];

/******************************************************************************/
/*                             Function implementation                        */
/******************************************************************************/

void tsk_i2c() {
	Change_speed_mutexHandle = osMutexNew(&Change_speed_mutex_attributes);

	motor_speed.speed_x.uint16Val = 0;
	motor_speed.speed_y.uint16Val = 0;
	motor_speed.speed_zoom.uint16Val = 0;

	if(HAL_I2C_EnableListen_IT(get_hi2cl()) != HAL_OK) {
		Error_Handler();
	}

	while(1){
		osDelay(10000);
	}
}

uint16_t get_speed_x() {
	uint16_t speed = 0;
	osMutexAcquire(Change_speed_mutexHandle, 0);

		speed = motor_speed.speed_x.uint16Val;

	osMutexRelease(Change_speed_mutexHandle);

	return speed;
}

uint16_t get_speed_y() {
	uint16_t speed = 0;
	osMutexAcquire(Change_speed_mutexHandle, 0);

		speed = motor_speed.speed_y.uint16Val;

	osMutexRelease(Change_speed_mutexHandle);

	return speed;
}

uint16_t get_speed_zoom() {
	uint16_t speed = 0;
	osMutexAcquire(Change_speed_mutexHandle, 0);

		speed = motor_speed.speed_zoom.uint16Val;

	osMutexRelease(Change_speed_mutexHandle);

	return speed;
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
	//add mutex
	if(aRxBuffer[0] == 0x10) {
		osMutexAcquire(Change_speed_mutexHandle, 0);
			motor_speed.speed_x.uint8Val[0] = aRxBuffer[1];
			motor_speed.speed_x.uint8Val[1] = aRxBuffer[2];
			motor_speed.speed_y.uint8Val[0] = aRxBuffer[3];
			motor_speed.speed_y.uint8Val[1] = aRxBuffer[4];
			motor_speed.speed_zoom.uint8Val[0] = aRxBuffer[5];
			motor_speed.speed_zoom.uint8Val[1] = aRxBuffer[6];
		osMutexRelease(Change_speed_mutexHandle);
	}
	//strcpy((char*) debug_buf, "SlaveRxCptl\r\n");
	//HAL_UART_Transmit(get_huart2(), debug_buf, strlen((char*)debug_buf), HAL_MAX_DELAY);
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
		if (HAL_I2C_Slave_Seq_Receive_IT(get_hi2cl(), (uint8_t *)aRxBuffer, RXBUFFERSIZE, I2C_NEXT_FRAME) != HAL_OK) {
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
