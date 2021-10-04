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

#define APP_DEBOUCING_TIME_MS 100
bool app_started = false;

void app_switch_interrupt(void)
{
	if(!app_started)
		return;

	hw_deboucing_timer_start();
}

void app_init(void)
{
	hw_timer_start();
	app_started = true;
}

void app_loop(void)
{
	hw_cpu_sleep();
}

