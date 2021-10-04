/*
 * hw.c
 *
 *  Created on: Oct 3, 2021
 *      Author: mikin
 */

#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "app.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim10;
//  84MHz clock
//  250ms = 4Hz
//  4Hz -> 42000(prescalar) e 500(ARR)
//  500ms = 2Hz
//  2Hz -> 42000(prescalar) e 1000(ARR)
uint32_t tim_values[] = { 499, 999 };
volatile uint32_t pos = 0;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_0)
	{
		app_switch_interrupt();
	}
}

 bool hw_switch_state_get(void)
 {
	GPIO_PinState switch_state = HAL_GPIO_ReadPin(USER_SW_GPIO_Port, USER_SW_Pin);

	//return switch_state == GPIO_PIN_SET ? true : false;

	if(switch_state == GPIO_PIN_SET)
		return true;
	else
		return false;
 }

 void hw_led_toggle(void)
 {
	 HAL_GPIO_TogglePin(USER_LED_GPIO_Port, USER_LED_Pin);
 }

 void hw_led_state_set(bool state)
 {
	 GPIO_PinState led_state = state ? GPIO_PIN_RESET : GPIO_PIN_SET;

	 HAL_GPIO_WritePin(USER_LED_GPIO_Port, USER_LED_Pin, led_state);

 }

 void hw_delay_ms(uint32_t time_ms)
 {
	 HAL_Delay(time_ms);
 }

 uint32_t hw_tick_ms_get(void)
 {
	 return HAL_GetTick();
 }

 void hw_cpu_sleep(void)
 {
	 __WFI();
 }

 void hw_timer_start(void)
 {
	 pos = 0;
	 __HAL_TIM_SET_AUTORELOAD(&htim1, tim_values[pos]);
	 __HAL_TIM_SET_COUNTER(&htim1, 0);

	 HAL_TIM_Base_Start_IT(&htim1);

 }

 void hw_toggle_led_time(void)
 {
	 pos++;

	 if(pos >= 2)
		 pos = 0;

	 __HAL_TIM_SET_AUTORELOAD(&htim1, tim_values[pos]);
	 __HAL_TIM_SET_COUNTER(&htim1, 0);
	 HAL_TIM_Base_Start_IT(&htim1);
 }

 void hw_deboucing_timer_start(void)
 {
	 //50ms deboucing = 20Hz
	 //logo: 50ms -> 42K(prescalar) 100 (ARR)
	 __HAL_TIM_SET_AUTORELOAD(&htim10, 100);
	 __HAL_TIM_SET_COUNTER(&htim10, 0);

	 HAL_TIM_Base_Stop_IT(&htim1);
	 HAL_TIM_Base_Start_IT(&htim10);
 }

 void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
 {
	 if (htim == &htim1)
		 hw_led_toggle();

	 if (htim == &htim10)
	 {
		 hw_toggle_led_time();
	 	 HAL_TIM_Base_Stop_IT(&htim10);
	 }
 }

