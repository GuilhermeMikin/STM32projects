/*
 * app.c
 *
 *  Created on: Sep 19, 2021
 *      Author: mikin
 */

#include <stdint.h>
#include <stdbool.h>
#include "app.h"
#include "hw.h"

#define APP_DEBOUCING_TIME_MS 240

volatile uint32_t led_time_on_ms = 100;
volatile uint32_t led_time_off_ms = 100;
bool app_started = false;
bool led_onfire = false;
bool switch_state = false;
bool switch2_state = false;

void app_switch_interrupt(void)
{
	static uint32_t deboucing_time_ms = 0;

	if((hw_tick_ms_get() - deboucing_time_ms) >= APP_DEBOUCING_TIME_MS)
	{
		if(switch_state == false)
			switch_state = true;
		else
			switch_state = false;

		deboucing_time_ms = hw_tick_ms_get();
	}
}

void app_switch2_interrupt(void)
{
	static uint32_t deboucing_time_ms = 0;

	if((hw_tick_ms_get() - deboucing_time_ms) >= APP_DEBOUCING_TIME_MS)
	{
		if(switch2_state == true)
			switch2_state = false;
		else
			switch2_state = true;

		deboucing_time_ms = hw_tick_ms_get();
	}
}

void truthtable_switch_state(void)
{
	if(switch_state && switch2_state)
	{
		led_time_on_ms = 900;
		led_time_off_ms = 450;
	}
	else if(switch_state && !switch2_state)
	{
		led_time_on_ms = 600;
		led_time_off_ms = 300;
	}
	else if(!switch_state && switch2_state)
	{
		led_time_on_ms = 300;
		led_time_off_ms = 150;
	}
	else
	{
		led_time_on_ms = 100;
		led_time_off_ms = 100;
	}
}

void app_tick_1ms(void)
{
	static uint32_t led_time_cnt_ms = 0;

	if(!app_started)
		return;

	truthtable_switch_state();

	led_time_cnt_ms++;

	if((led_time_cnt_ms >= led_time_off_ms) && (led_onfire == false))
	{
		hw_led_toggle();
		led_onfire = true;
		led_time_cnt_ms = 0;
	}
	else if((led_time_cnt_ms >= led_time_on_ms) && (led_onfire == true))
	{
		hw_led_toggle();
		led_onfire = false;
		led_time_cnt_ms = 0;
	}

}

void app_init(void)
{
	app_started = true;
}

void app_loop(void)
{
	hw_cpu_sleep();
	app_tick_1ms();
}
