#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/periph_ctrl.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "driver/pcnt.h"
#include "esp_attr.h"
#include "esp_log.h"

#define PCNT_UNIT_LEFT PCNT_UNIT_0
#define PCNT_UNIT_RIGHT PCNT_UNIT_1
#define PCNT_CHANNEL_LEFT PCNT_CHANNEL_0
#define PCNT_CHANNEL_RIGHT PCNT_CHANNEL_1
#define LEFT_ENCODER_PULSE 17
#define RIGHT_ENCODER_PULSE 18
#define LEFT_ENCODER_CTRL
#define RIGHT_ENCODER_CTRL

int counter_left = 0, counter_right = 0;

static void pcnt_config_init()
{
	pcnt_config_t pcnt_config_left = {
		// Set PCNT input signal and control GPIOs
		.pulse_gpio_num = LEFT_ENCODER_PULSE,
		.ctrl_gpio_num = LEFT_ENCODER_CTRL,
		.channel = PCNT_CHANNEL_LEFT,
		.unit = PCNT_UNIT_LEFT,
		.pos_mode = PCNT_COUNT_INC,
		.neg_mode = PCNT_COUNT_DIS,
		// .lctrl_mode??
		// .hctrl_mode??
	}

	pcnt_config_t pcnt_config_right = {
		// Set PCNT input signal and control GPIOs
		.pulse_gpio_num = RIGHT_ENCODER_PULSE.
		.ctrl_gpio_num = RIGHT_ENCODER_CTRL,
		.channel = PCNT_CHANNEL_RIGHT,
		.unit = PCNT_UNIT_RIGHT,
		.pos_mode = PCNT_COUNT_INC,
		.neg_mode = PCNT_COUNT_DIS,
		// .lctrl_mode???
		// .hctrl_mode???
	}

	pcnt_unit_config(&pcnt_config_left);
	pcnt_unit_config(&pcnt_config_right);

	pcnt_counter_pause(PCNT_UNIT_LEFT);
	pcnt_counter_pause(PCNT_UNIT_RIGHT);
	pcnt_counter_clear(PCNT_UNIT_LEFT);
	pcnt_counter_clear(PCNT_UNIT_RIGHT);

	pcnt_counter_resume(PCNT_UNIT_LEFT);
	pcnt_counter_resume(PCNT_UNIT_RIGHT);


} 

void app_main()
{
	pcnt_config_init();

	while(1)
	{
		pcnt_get_counter_value(PCNT_UNIT_LEFT, &counter_left);
		pcnt_get_counter_value(PCNT_UNIT_RIGHT, &counter_right);
		printf("L COUNT:\t%d\t", counter_left);
		printf("R COUNT:\t%d\n", counter_right);
	}

}