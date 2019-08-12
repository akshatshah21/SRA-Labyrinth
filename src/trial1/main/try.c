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

void distance_count() {
	printf("Hello\n");
	vTaskDelay(1000 / portTICK_PERIOD_MS);
}

void app_main() {
	    xTaskCreate(&distance_count,"distance_count",4096,NULL,1,NULL);

}