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

int counter_left = 0;
int counter_right = 0;



void countToDistance()
{
    double count_average,distance_final;
    count_average = ((double)counter_left + counter_right)/2;
    printf("Average value of counter %g",count_average);
    distance_final = (count_average * 20.3)/20;
    printf("Final distance is: %g",distance_final);

}

void http_netconn_serve(struct netconn *conn) {
    const static char http_html_hdr[] = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";
    const static char http_index_hml[] = "<html><head><title>Encoder distance</title></head><body></body></html>";
    char dist[5];   // char array for storing distance

    netconn_write(conn, http_html_hdr, sizeof(http_html_hdr)-1, NETCONN_NOCOPY);
    netconn_write(conn, http_index_hml, sizeof(http_index_hml)-1, NETCONN_NOCOPY);
    netconn_write(conn, &counter_left, 4, NETCONN_NOCOPY);
    netconn_close(conn);
}

void http_server()
{
    struct netconn *conn, *newconn;
    err_t err;
    conn = netconn_new(NETCONN_TCP);
    netconn_bind(conn, NULL, 80);
    netconn_listen(conn);
    do {
        err = netconn_accept(conn, &newconn);   
        if(err == ERR_OK) {
            http_netconn_serve(newconn); //Serve distance
            netconn_delete(newconn);
        }
    } while(err == ERR_OK);
    netconn_close(conn);
    netconn_delete(conn);
}

void distance_count_left(void *arg)
{
	/*
		Set the The LED Pins : GPIO 0 and GPIO 5 to OUTPUT
	*/
     
    gpio_set_direction(left,GPIO_MODE_INPUT);
    //gpio_set_direction(right,GPIO_MODE_INPUT);
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
                printf("%d\n", counter_left);
                //vTaskDelay(100/portTICK_PERIOD_MS);
				break;
            }
        }
        

		// http_server();
	}
	
}

void distance_count_right(void *arg)
{
	/*
		Set the The LED Pins : GPIO 0 and GPIO 5 to OUTPUT
	*/
     
    //gpio_set_direction(left,GPIO_MODE_INPUT);
    gpio_set_direction(right,GPIO_MODE_INPUT);
	//gpio_set_direction(LED_1,GPIO_MODE_OUTPUT);
	//gpio_set_direction(LED_2,GPIO_MODE_OUTPUT);	

	while(1)
	{
        while(gpio_get_level(right) == 0)
        {   

            while(gpio_get_level(right) == 1)
            {
                counter_right++;
		        //gpio_set_level(LED_1,0);	//Set LED1 ON
		        //gpio_set_level(LED_2,0);	//Set LED2 ON
                printf("count for right is:");
                printf("%d\n", counter_right);
                //vTaskDelay(100/portTICK_PERIOD_MS);
				break;
            }
        }

		// http_server();
	}
	
}
void app_main()
{
	/*
		Basic Function for task creation
	*/


	// initialize_wifi();
    
    // const esp_mqtt_client_config_t client = {
    //     .uri = "postman.cloudmqtt.com",
    //     .username = "ocbshoyv".
    //     .password= "u7RF9Xts1g1r",
    //     .port = 16557
    // };

    xTaskCreatePinnedToCore(&distance_count_left,"distance_count_left",4096,NULL,1,NULL,0);
	xTaskCreatePinnedToCore(&distance_count_right,"distance_count_right",4096,NULL,1,NULL,1);
}