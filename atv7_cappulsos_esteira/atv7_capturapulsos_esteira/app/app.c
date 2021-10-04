/*
 * app.c
 *
 *  Created on: Oct 3, 2021
 *      Author: mikin
 */

#include <stdint.h>
#include <stdbool.h>
#include "app.h"
#include "hw.h"

#define APP_DEBOUCING_TIME_MS 250

volatile uint32_t led_time_ms = 100;
bool app_started = false;
volatile bool esteira_livre = true;

void app_switch_interrupt(void)
{
	static uint32_t deboucing_time_ms = 0;

	if(!app_started)
		return;

	if((esteira_livre == false) && ((hw_tick_ms_get() - deboucing_time_ms) >= APP_DEBOUCING_TIME_MS))
	{
		esteira_livre = true;
		hw_led_state_set(false);
		deboucing_time_ms = hw_tick_ms_get();
	}
}

void app_init(void)
{
	hw_input_capture_init();
	app_started = true;
	hw_led_state_set(false);
}

void app_loop(void)
{
	if (esteira_livre)
	{
		uint16_t cnt = hw_input_capture_get();
		if(cnt >= 100)
		{
			hw_input_capture_set(0);
			esteira_livre = false;
			hw_led_state_set(true);
		}
	}
	else
		hw_cpu_sleep();
}

