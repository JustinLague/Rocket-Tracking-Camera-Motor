/*
 * motor.c
 *
 *  Created on: Mar 31, 2023
 *      Author: Justin
 */

/******************************************************************************/
/*                             Includes	                                      */
/******************************************************************************/
#include "main.h"
#include "motor.h"
#include <math.h>

/******************************************************************************/
/*                             Function definition                            */
/******************************************************************************/

Timer_def_t timer_x;
Timer_def_t timer_y;
Timer_def_t timer_zoom;

void tsk_motor_control() {

	start_motor(Motor_x);
	start_motor(Motor_y);
	start_motor(Motor_zoom);

	uint32_t speed_x = 60;
	uint32_t speed_y = 60;
	uint32_t speed_zoom = 60;


	while(speed_x <= 3000) {
		update_motor_x(speed_x);

		speed_x +=45;
		osDelay(5);
	}

	update_motor_x(speed_x);

	osDelay(1000);

	while(speed_x >= 60) {
		update_motor_x(speed_x);

		speed_x -=10;
		osDelay(5);
	}

	update_motor_x(speed_x);
}


void update_motor_x(uint32_t speed) {
	change_speed(&timer_x, speed);
	TIM3->ARR = timer_x.arr;
	TIM3->PSC = timer_x.psc;
	TIM3->CCR2 = (uint32_t) floor(timer_x.arr / 2);
}

void update_motor_y(uint32_t speed) {
	change_speed(&timer_y, speed);
	TIM4->ARR = timer_y.arr;
	TIM4->PSC = timer_y.psc;
	TIM4->CCR3 = (uint32_t) floor(timer_y.arr / 2);
}

void update_motor_zoom(uint32_t speed) {
	change_speed(&timer_zoom, speed);
	TIM8->ARR = timer_zoom.arr;
	TIM8->PSC = timer_zoom.psc;
	TIM8->CCR4 = (uint32_t) floor(timer_zoom.arr / 2);
}

void change_timer_def_for_freq(uint32_t desired_freq, Timer_def_t* timer_def) {

	//frequence du PCLK1
	uint32_t f_clk = HAL_RCC_GetPCLK1Freq();

	// formule : https://deepbluembedded.com/stm32-pwm-example-timer-pwm-mode-tutorial/ apres je solve pour ARR
	uint32_t arr = (f_clk / desired_freq);
	uint32_t psc = 0;

	if(arr > 65535) {
		psc = (uint32_t) floor(arr / 65535) + 1;
		arr = arr / psc;
	}

	timer_def->arr = arr - 1;
	timer_def->psc = psc == 0 ? psc : psc - 1;
}

// go to X rpm
void change_speed(Timer_def_t* timer_def, uint32_t speed) {
	//TODO: change "400" according to mode 1/mode * 400;

	uint32_t freq = (speed * 400) / 60;
	change_timer_def_for_freq(freq, timer_def);
}
