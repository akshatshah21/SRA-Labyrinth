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

#define left 17
#define right 18

void distance_count(void *arg)
{
	/*
		Set the The LED Pins : GPIO 0 and GPIO 5 to OUTPUT
	*/
    int counter_left = 0;
    int counter_right = 0; 
    gpio_set_direction(left,GPIO_MODE_INPUT);
    gpio_set_direction(right,GPIO_MODE_INPUT);
	//gpio_set_direction(LED_1,GPIO_MODE_OUTPUT);
	//gpio_set_direction(LED_2,GPIO_MODE_OUTPUT);	

	while(1)
	{
        while(gpio_get_level(left) == 0)
        {   

            while(gpio_get_level(left) == 1)
            {
                counter_left++;
		        //gpio_set_level(LED_1,0);	//Set LED1 ON
		        //gpio_set_level(LED_2,0);	//Set LED2 ON
                printf("count for left is:");
                printf(counter_left);
                //vTaskDelay(100/portTICK_PERIOD_MS);
            }
        }
        while(gpio_get_level(right) == 0)
        {   

            while(gpio_get_level(right) == 1)
            {
                counter_right++;
		        //gpio_set_level(LED_1,0);	//Set LED1 ON
		        //gpio_set_level(LED_2,0);	//Set LED2 ON
                printf("count for right is:");
                printf(counter_right);
                //vTaskDelay(100/portTICK_PERIOD_MS);
            }
        }
	}
	
}

void countToDistance(counter_left,counter_right)
{
    double count_average,distance_final;
    count_average = (counter_left + counter_right)/2;
    printf("%g","Average value of counter",count_average);
    distance_final = (count_average * 20.3)/20;
    printf("%g","Final distance is:",distance_final);

}
void app_main()
{
	/*
		Basic Function for task creation
	*/

    xTaskCreate(&distance_count,"distance_count",4096,NULL,1,NULL);
}