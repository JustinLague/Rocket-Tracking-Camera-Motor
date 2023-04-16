// /*
//  * i2c.c
//  *
//  *  Created on: Apr 2, 2023
//  *      Author: Justin
//  */

// /******************************************************************************/
// /*                             Includes	                                      */
// /******************************************************************************/
// #include "main.h"
// #include "i2c.h"

// /******************************************************************************/
// /*                             Global variable                                */
// /******************************************************************************/
// motor_error_t motor_error;

// uint8_t RxBuffer[4];
// uint8_t TxBuffer[4];

// uint32_t Transfer_Direction = 0;

// #define RXBUFFERSIZE                      (COUNTOF(RxBuffer))
// #define TXBUFFERSIZE                      (COUNTOF(TxBuffer))
// #define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

// /******************************************************************************/
// /*                             Function definition                            */
// /******************************************************************************/

// void init_I2c() {
// 	motor_error.error_x = 0.0f;
// 	motor_error.error_y = 0.0f;
// 	motor_error.error_zoom = 0.0f;

// 	RxBuffer[0] = 0x00;
// 	RxBuffer[1] = 0x00;
// 	RxBuffer[2] = 0x00;
// 	RxBuffer[3] = 0x00;


// 	TxBuffer[0] = 0x10;
// 	TxBuffer[1] = 0x20;
// 	TxBuffer[2] = 0x30;
// 	TxBuffer[3] = 0x30;

// 	 if(HAL_I2C_EnableListen_IT(get_hi2cl()) != HAL_OK)
// 	 {
// 		/* Transfer error in reception process */
// 		Error_Handler();
// 	 }

// 	 HAL_I2C_Slave_Seq_Receive_IT(get_hi2cl(),(uint8_t*)RxBuffer, RXBUFFERSIZE, I2C_FIRST_AND_LAST_FRAME);
// }

// void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *I2cHandle) {
// 	set_Xfer_Complete(1);
// }

// void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *I2cHandle) {
// 	set_Xfer_Complete(1);

// 	TxBuffer[0]++;
// 	TxBuffer[1]++;
// 	TxBuffer[2]++;
// 	TxBuffer[3]++;
// }


// void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode) {
// 	Transfer_Direction = TransferDirection;
// 	if (Transfer_Direction != 0) {
// 		/*##- Start the transmission process #####################################*/
// 		/* While the I2C in reception process, user can transmit data through
// 		 "TxBuffer" buffer */
// 		if (HAL_I2C_Slave_Seq_Transmit_IT(get_hi2cl(), (uint8_t *)TxBuffer, TXBUFFERSIZE, I2C_FIRST_AND_LAST_FRAME) != HAL_OK) {
// 			/* Transfer error in transmission process */
// 			Error_Handler();
// 		}

// 	} else {
// 		/*##- Put I2C peripheral in reception process ###########################*/
// 		if (HAL_I2C_Slave_Seq_Receive_IT(get_hi2cl(), (uint8_t *)RxBuffer, RXBUFFERSIZE, I2C_FIRST_AND_LAST_FRAME) != HAL_OK) {
// 			/* Transfer error in reception process */
// 			Error_Handler();
// 		}
// 	}
// }

// void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
// {
// }

// void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *I2cHandle) {
//   /** Error_Handler() function is called when error occurs.
//     * 1- When Slave doesn't acknowledge its address, Master restarts communication.
//     * 2- When Master doesn't acknowledge the last data transferred, Slave doesn't care in this example.
//     */

// 	uint32_t err = HAL_I2C_GetError(I2cHandle);

// 	if (HAL_I2C_GetError(I2cHandle) != HAL_I2C_ERROR_AF)
// 	{
// 		Error_Handler();
// 	}
// }

