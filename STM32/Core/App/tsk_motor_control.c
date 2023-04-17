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

Timer_def_t timer_x;
Timer_def_t timer_y;
Timer_def_t timer_zoom;

uint16_t actual_speed_x = 0;
uint16_t actual_speed_y = 0;
uint16_t actual_speed_zoom = 0;

uint16_t acceleration = 2;

void tsk_motor_control() {

	start_motor(Motor_x);
	start_motor(Motor_y);
	start_motor(Motor_zoom);

	while(1){
		update_motor_x();
		update_motor_y();
		update_motor_zoom();
		osDelay(2);
	}
}


void update_motor_x() {
	uint16_t desired_speed = get_speed_x();

	if(actual_speed_x < desired_speed) {
		actual_speed_x += acceleration;
	} else if(actual_speed_x > desired_speed) {
		actual_speed_x -= acceleration;
	}

	change_speed(&timer_x, actual_speed_x);
	osKernelLock();
		TIM3->ARR = timer_x.arr;
		TIM3->PSC = timer_x.psc;
		TIM3->CCR2 = (uint32_t) floor(timer_x.arr / 2);
	osKernelUnlock();
}

void update_motor_y() {
	uint16_t desired_speed = get_speed_y();
	if(actual_speed_y < desired_speed) {
		actual_speed_y += acceleration;
	} else if (actual_speed_y > desired_speed) {
		actual_speed_y -= acceleration;
	}

	change_speed(&timer_y, actual_speed_y);

	osKernelLock();
		TIM4->ARR = timer_y.arr;
		TIM4->PSC = timer_y.psc;
		TIM4->CCR3 = (uint32_t) floor(timer_y.arr / 2);
	osKernelUnlock();
}

void update_motor_zoom() {
	uint16_t desired_speed = get_speed_zoom();

	if(actual_speed_zoom < desired_speed) {
		actual_speed_zoom += acceleration;
	} else if(actual_speed_zoom > desired_speed) {
		actual_speed_zoom -= acceleration;
	}

	change_speed(&timer_zoom, actual_speed_zoom / 2);

	osKernelLock();
		TIM8->ARR = timer_zoom.arr;
		TIM8->PSC = timer_zoom.psc;
		TIM8->CCR4 = (uint32_t) floor(timer_zoom.arr / 2);
	osKernelUnlock();
}

void change_timer_def_for_freq(uint32_t desired_freq, Timer_def_t* timer_def) {

	//frequence du PCLK1
	uint32_t f_clk = HAL_RCC_GetPCLK1Freq();

	// formule : https://deepbluembedded.com/stm32-pwm-example-timer-pwm-mode-tutorial/ apres je solve pour ARR
	uint32_t arr = 0;

	if(desired_freq != 0) {
		arr = (f_clk / desired_freq);
	}
	uint32_t psc = 0;

	if(arr > 65535) {
		psc = (uint32_t) floor(arr / 65535) + 1;
		arr = arr / psc;
	}

	timer_def->arr = arr - 1;
	timer_def->psc = psc == 0 ? psc : psc - 1;
}

// go to X rpm
void change_speed(Timer_def_t* timer_def, uint16_t speed) {
	//TODO: change "400" according to mode 1/mode * 400;

	uint32_t freq = (speed * (4 * 400)) / 60;
	change_timer_def_for_freq(freq, timer_def);
}
