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

extern 	TIM_HandleTypeDef htim1;

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

void hw_input_capture_init(void)
{
	HAL_TIM_Base_Start(&htim1);
	hw_input_capture_set(0);
}

uint16_t hw_input_capture_get(void)
{
	return __HAL_TIM_GET_COUNTER(&htim1);
}

void hw_input_capture_set(uint16_t cnt)
{
	__HAL_TIM_SET_COUNTER(&htim1, cnt);
}

