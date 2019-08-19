//C Headers
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

//Components
#include "MPU.h"
#include "SRA18.h"
#include "TUNING.h"


adc1_channel_t ls_ahead_channel = ADC_CHANNEL_4;	// 32
adc1_channel_t ls_left_channel = ADC_CHANNEL_5;		// 33
adc2_channel_t ls_right_channel = ADC_CHANNEL_4;	// 13

void adc_ls(void *arg)
{
	while(1)
	{
		printf("RAW1: %d  ", adc1_get_raw(ls_ahead_channel));
		printf("RAW2: %d\n", adc1_get_raw(ls_left_channel));
		// printf("RAW3: %d  ", adc2_get_raw(ls_right_channel));
	}

}

void app_main()
{
	xTaskCreate(&adc_ls, "adc_ls", 4096, NULL, 1, NULL);
}