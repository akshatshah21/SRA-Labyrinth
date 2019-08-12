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

#define LS_LEFT 25
#define LS_RIGHT 26
// #define LS_AHEAD 32

void blink_task1(void *arg)
{
	/*
		Set the The LED Pins : GPIO 0 and GPIO 5 to OUTPUT
	*/

    gpio_set_direction(LS_LEFT,GPIO_MODE_INPUT);
	// gpio_set_direction(LS_RIGHT,GPIO_MODE_INPUT);
	// gpio_set_direction(LS,GPIO_MODE_INPUT);
	gpio_set_direction(LED_1,GPIO_MODE_OUTPUT);
	// gpio_set_direction(LED_2,GPIO_MODE_OUTPUT);
	

	while(1)
	{
        if(gpio_get_level(LS_LEFT) == 1)
        {
		gpio_set_level(LED_1,0);	//Set LED1 ON
		// gpio_set_level(LED_2,0);	//Set LED2 ON
        printf("LS LEFT:\tBLACK\n");
        vTaskDelay(50/portTICK_PERIOD_MS);
        }
        else
        {
		gpio_set_level(LED_1,1);	//Set LED1 OFF
		// gpio_set_level(LED_2,1);	//Set LED2 OFF
        printf("LS LEFT:\tWHITE\n");
        vTaskDelay(50/portTICK_PERIOD_MS);
        }
	}
	
}


void blink_task2(void *arg)
{
	/*
		Set the The LED Pins : GPIO 0 and GPIO 5 to OUTPUT
	*/

    // gpio_set_direction(LS_LEFT,GPIO_MODE_INPUT);
	gpio_set_direction(LS_RIGHT,GPIO_MODE_INPUT);
	// gpio_set_direction(LS,GPIO_MODE_INPUT);
	// gpio_set_direction(LED_1,GPIO_MODE_OUTPUT);
	gpio_set_direction(LED_2,GPIO_MODE_OUTPUT);
	

	while(1)
	{
        if(gpio_get_level(LS_RIGHT) == 1)
        {
		// gpio_set_level(LED_1,0);	//Set LED1 ON
		gpio_set_level(LED_2,0);	//Set LED2 ON
        printf("LS RIGHT:\tBLACK\n");
        vTaskDelay(50/portTICK_PERIOD_MS);
        }
        else
        {
		// gpio_set_level(LED_1,1);	//Set LED1 OFF
		gpio_set_level(LED_2,1);	//Set LED2 OFF
        printf("LS RIGHT:\tWHITE\n");
        vTaskDelay(50/portTICK_PERIOD_MS);
        }
	}
	
}

void app_main()
{
	/*
		Basic Function for task creation
	*/

    xTaskCreate(&blink_task1,"blink task1",4096,NULL,1,NULL);
	xTaskCreate(&blink_task2, "blink_task2", 4096, NULL, 1, NULL);
}
