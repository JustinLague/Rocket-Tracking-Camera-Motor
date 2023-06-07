/*
 * tsk_motor_control.c
 *
 *  Created on: Mar 31, 2023
 *      Author: Justin
 */

/******************************************************************************/
/*                             Includes	                                      */
/******************************************************************************/
#include "main.h"
#include "tsk_motor_control.h"
#include <math.h>
#include "cmsis_os2.h"
#include <string.h>
#include "i2c.h"

/******************************************************************************/
/*                             Function implementation                        */
/******************************************************************************/
void tsk_motor_control() {
	start_motor_pwm(MOTOR_X);
	start_motor_pwm(MOTOR_Y);
	start_motor_pwm(MOTOR_ZOOM);

	while(1){
		update_motor(MOTOR_Y);
		update_motor(MOTOR_ZOOM);
		update_motor(MOTOR_X);
		osDelay(2);
	}
}

void start_motor_pwm(enum motor_e motor) {
	switch(motor){
		case MOTOR_X:
			HAL_TIM_PWM_Start(get_htim3(), TIM_CHANNEL_2);
			TIM2->PSC = 0;
			break;
		case MOTOR_Y:
			HAL_TIM_PWM_Start(get_htim4(), TIM_CHANNEL_3);
			TIM3->PSC = 0;
			break;
		case MOTOR_ZOOM:
			HAL_TIM_PWM_Start(get_htim8(), TIM_CHANNEL_4);
			TIM4->PSC = 0;
			break;
	}
}

void stop_motor_pwm(enum motor_e motor) {
	switch(motor){
		case MOTOR_X:
			HAL_TIM_PWM_Stop(get_htim3(), TIM_CHANNEL_2);
			break;
		case MOTOR_Y:
			HAL_TIM_PWM_Stop(get_htim4(), TIM_CHANNEL_3);
			break;
		case MOTOR_ZOOM:
			HAL_TIM_PWM_Stop(get_htim8(), TIM_CHANNEL_4);
			break;
	}
}

void update_motor(enum motor_e _motor) {
	motor_t* motor = get_motor(_motor);

	if(motor->actual_speed < motor->desired_speed.uint16Val) {
		motor->actual_speed += motor->max_acceleration.uint16Val;
	} else if(motor->actual_speed > motor->desired_speed.uint16Val) {
		motor->actual_speed -= motor->max_acceleration.uint16Val;
	}

	update_pwm(motor);
	update_mode(motor->mode, motor->type);
	update_dir(motor->dir, motor->type);
}

void update_mode(uint8_t mode, enum motor_e _motor){

	switch(_motor) {
		case MOTOR_X:
			HAL_GPIO_WritePin(MODE_X_1_GPIO_Port, MODE_X_1_Pin, (mode & 0b100) >> 2);
			HAL_GPIO_WritePin(MODE_X_2_GPIO_Port, MODE_X_2_Pin, (mode & 0b010) >> 1);
			HAL_GPIO_WritePin(MODE_X_3_GPIO_Port, MODE_X_3_Pin, mode & 0b001);
			break;
		case MOTOR_Y:
			HAL_GPIO_WritePin(MODE_Y_1_GPIO_Port, MODE_Y_1_Pin, (mode & 0b100) >> 2);
			HAL_GPIO_WritePin(MODE_Y_2_GPIO_Port, MODE_Y_2_Pin, (mode & 0b010) >> 1);
			HAL_GPIO_WritePin(MODE_Y_3_GPIO_Port, MODE_Y_3_Pin, mode & 0b001);
			break;
		case MOTOR_ZOOM:
			HAL_GPIO_WritePin(MODE_ZOOM_1_GPIO_Port, MODE_ZOOM_1_Pin, (mode & 0b100) >> 2);
			HAL_GPIO_WritePin(MODE_ZOOM_2_GPIO_Port, MODE_ZOOM_2_Pin, (mode & 0b010) >> 1);
			HAL_GPIO_WritePin(MODE_ZOOM_3_GPIO_Port, MODE_ZOOM_3_Pin, mode & 0b001);
			break;
	}
}

void update_dir(uint8_t dir, enum motor_e _motor) {
	switch(_motor) {
			case MOTOR_X:
				HAL_GPIO_WritePin(DIR_MOTOR_X_GPIO_Port, DIR_MOTOR_X_Pin, dir);
				break;
			case MOTOR_Y:
				HAL_GPIO_WritePin(DIR_MOTOR_Y_GPIO_Port, DIR_MOTOR_Y_Pin, dir);
				break;
			case MOTOR_ZOOM:
				HAL_GPIO_WritePin(DIR_MOTOR_ZOOM_GPIO_Port, DIR_MOTOR_ZOOM_Pin, dir);
				break;
	}
}

void update_pwm(motor_t* motor) {

	//need to find a way to compute mode
	uint32_t pwm_frequency = (motor->actual_speed * (1 * 400)) / 60;

	update_timer(pwm_frequency, &motor->timer);

	switch(motor->type) {
		case MOTOR_X:
			osKernelLock();
				TIM3->ARR = motor->timer.arr;
				TIM3->PSC = motor->timer.psc;
				TIM3->CCR2 = (uint32_t) floor(motor->timer.arr / 2);
			osKernelUnlock();
			break;
		case MOTOR_Y:
			osKernelLock();
				TIM4->ARR = motor->timer.arr;
				TIM4->PSC = motor->timer.psc;
				TIM4->CCR3 = (uint32_t) floor(motor->timer.arr / 2);
			osKernelUnlock();
			break;
		case MOTOR_ZOOM:
			osKernelLock();
				TIM8->ARR = motor->timer.arr;
				TIM8->PSC = motor->timer.psc;
				TIM8->CCR4 = (uint32_t) floor(motor->timer.arr / 2);
			osKernelUnlock();
			break;
	}
}

void update_timer(uint32_t pwm_frequency, Timer_def_t* timer) {

	//frequence du PCLK1
	uint32_t f_clk = HAL_RCC_GetPCLK1Freq();

	// formule : https://deepbluembedded.com/stm32-pwm-example-timer-pwm-mode-tutorial/ apres je solve pour ARR
	uint32_t arr = 0;

	if(pwm_frequency != 0) {
		arr = (f_clk / pwm_frequency);
	}
	uint32_t psc = 0;

	if(arr > 65535) {
		psc = (uint32_t) floor(arr / 65535) + 1;
		arr = arr / psc;
	}

	timer->arr = arr - 1;
	timer->psc = psc == 0 ? psc : psc - 1;
}


