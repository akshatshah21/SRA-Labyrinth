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
//esp pins
#define left 17
#define right 18

#define LED_0 0
#define LED_1 5
adc1_channel_t ls_ahead_channel = ADC_CHANNEL_4;	// 32
adc1_channel_t ls_left_channel = ADC_CHANNEL_5;		// 33
adc2_channel_t ls_right_channel = ADC2_CHANNEL_4;	// 13
adc2_config_channel_atten(ADC2_CHANNEL_4,ADC_ATTEN_DB_11);

// int counter_left = 0;
// int counter_right = 0;

adc1_channel_t channel[4] = {ADC_CHANNEL_7, ADC_CHANNEL_6, ADC_CHANNEL_0, ADC_CHANNEL_3};
int weights[4] = {-3,-1,1,3};

/*
* Line Following PID Variables
*/
float error=0, prev_error, difference, cumulative_error, correction;
int adc_reading[4];
float sensor_value[4];
float sensor_count;


/*
    * PID constants for correction for line following
	1.* kp = 0.6 and kd=0.1 opt=67
	2.kp = 0.55 kd = 0.3 opt=70
	3. kp = 0.45 kd = 0.3 opt = 75
	right_pwm  = 83
*/
float kp= 0.55;
float ki= 0;
float kd= 0.3;

//...................................
/* Motor value constraints
*/
float optleft = 70, optright = 70;
float lower_pwm_constrain = 60;
float higher_pwm_constrain = 90;
float left_pwm = 0, right_pwm = 0;

//extra declararion
float setpoint = 0,pitch_kP = 0,pitch_kD = 0,pitch_kI = 0,forward_buffer = 0,forward_offset = 0;


/*
 * Function to get analog readings from Line Sensors in LSA
 */
static void read_sensors()
{
    for(int i = 0; i < 4; i++)
    {
    adc_reading[i] = adc1_get_raw(channel[i]); //get the direct reading from sensors}
    }
}

/*
 * Function to map and constrain LSA values in a scale of 0 to 1000
 */ 
static void calc_sensor_values()
{
    // now constrain sensor reading between 0 to 1000
    for(int i = 0; i < 4; i++)
        {
            sensor_value[i] = map(adc_reading[i], 1700, 4000, 0, 1000);
            sensor_value[i] = constrain(sensor_value[i],0,1000);
        }
}

/*
 * Function to calculate the error from mapped sensor values
 */ 
static void calc_error()
{
    int all_black_flag = 1;
    long int weighted_sum = 0, sum = 0, pos = 0;
    
    for(int i = 0; i < 4; i++)
    {
        if(sensor_value[i] > 400)  // when bot will be in white region
            all_black_flag = 0;
        
        if(sensor_value[i] > 10)
        {
            weighted_sum += (long)(sensor_value[i]) * (weights[i]*1000);
            sum += sensor_value[i];
        }
    }
    
    if(sum != 0)
    {
        pos = weighted_sum / sum;
    }

    if(all_black_flag == 1)
    {
        if(error > 0)
            pos = 2500;
        else
            pos = -2500;
    }

    error = pos;    // Since required position is 0, so error = current_pos - 0 = current_pos
}
 
/*
 * Function to calculate the correction from error using PID 
 * Implements PID control
 */ 
static void calc_correction()
{
error *= 0.01;
difference = error - prev_error;    // Derivative
cumulative_error += error;          // Integrating error
correction = kp*error + ki*cumulative_error + kd*difference; //PID formula //ADD ALL...
prev_error = error;                 //For calculation of derivative term

//Work out manually, with the convention, to understand why + in left_pwm and - in right_pwm. 
left_pwm = constrain((optleft + correction), lower_pwm_constrain, higher_pwm_constrain) ;
right_pwm = constrain((optright - correction), lower_pwm_constrain, higher_pwm_constrain)  ;
//printf("right pwm: %g",right_pwm);
//printf("left pwm: %g",left_pwm);
}

/*
 * Function to use ESP32 WiFi to create server to control PID constants
 * Has PID constants for self-balancing also
 */ 
// void http_server(void *arg)
// {
//     printf("%s\n", "http task");
//     struct netconn *conn, *newconn;
//     err_t err;
//     conn = netconn_new(NETCONN_TCP);
//     netconn_bind(conn, NULL, 80);
//     netconn_listen(conn);
//     do {
//      err = netconn_accept(conn, &newconn);
//      if (err == ERR_OK) {
//        http_server_netconn_serve(newconn,&setpoint,&pitch_kP,&pitch_kD,&pitch_kI,&kp,&kd,&ki, &forward_offset, &forward_buffer);
//        netconn_delete(newconn);
//      }
//     } while(err == ERR_OK);
//     netconn_close(conn);
//     netconn_delete(conn);
// }

/*
 * Function for line following, passed in xTaskCreate() in app_main()
 */ 
void junction()
{	int left_tom = adc1_get_raw(ls_left_channel);
	int ahead_tom = adc1_get_raw(ls_ahead_channel);
	int right_tom = 0;
	int *abc;
	abc = &right_tom;
	adc2_get_raw(ls_right_channel,3,*abc);
	// gpio_set_direction(LS_LEFT,GPIO_MODE_INPUT);
	// gpio_set_direction(LS_RIGHT,GPIO_MODE_INPUT);
	// gpio_set_direction(LS_AHEAD,GPIO_MODE_INPUT);
	printf("L: %d\n",left_tom);
	printf("A: %d\n",ahead_tom);
	printf("R: %d\n",right_tom);
	if(left_tom >= 3500 && ahead_tom >=3500 && right_tom >= 3500)
	{
		printf("Condition 0\n");
		bot_stop(MCPWM_UNIT_0, MCPWM_TIMER_0);
		vTaskDelay(500/portTICK_PERIOD_MS);
		bot_spot_left(MCPWM_UNIT_0,MCPWM_TIMER_0,70,70);//left means right
		vTaskDelay(30);
	}
	else if(left_tom >= 3500 && ahead_tom >=3500)
	{
		printf("Condition 1\n");
		gpio_set_level(LED_0, 0);
		bot_stop(MCPWM_UNIT_0, MCPWM_TIMER_0);
		vTaskDelay(500/portTICK_PERIOD_MS);	
		bot_forward(MCPWM_UNIT_0,MCPWM_TIMER_0,70,70);
		vTaskDelay(50);


	}
	// else if(gpio_get_level(LS_AHEAD) == 1)
	// {
	// 	bot_spot_right(MCPWM_UNIT_0,MCPWM_TIMER_0,70,70);
	
	// }
	else if(left_tom >= 3500)
	{
		printf("Condition 2\n");
		gpio_set_level(LED_1, 0);
		bot_stop(MCPWM_UNIT_0, MCPWM_TIMER_0);
		vTaskDelay(500/portTICK_PERIOD_MS);
		bot_spot_right(MCPWM_UNIT_0,MCPWM_TIMER_0,70,70);// spot_right matlab left turn
		vTaskDelay(30);
	}
	else
	{
		printf("condition3 \n");
	
	
		
	}
	gpio_set_level(LED_1, 1);
	gpio_set_level(LED_0, 1);
	

}
void line_follow(void *arg)
{

     vTaskDelay(100/ portTICK_RATE_MS);
    while(1)
    {
		// if(counter_right>=40)
		// {	
		// 	bot_stop(MCPWM_UNIT_0, MCPWM_TIMER_0);
		// 	vTaskDelay(10000);
		// }
        // get the raw readings
        read_sensors();
        // Calculate sensor reading in 0 to 1000
        calc_sensor_values();
        // Calculate error in position w.r.t. line
        calc_error();
        // Calculate the correction using PID formula and implement PID control
        calc_correction();
        // Every iteration will send a signal to the motors to keep moving forward 
        bot_forward(MCPWM_UNIT_0, MCPWM_TIMER_0, left_pwm, right_pwm);
		junction();
        // Print values to monitor for testing
        // printf("Error: %f\t",error );
        // printf("correction: %f ",correction);
        // printf("Left Pwm: %f\t",left_pwm );
        // printf("Right Pwm: %f\n",right_pwm );  
    }
}
//function for distance fin
// void countToDistance()
// {
//     double count_average,distance_final;
//     count_average = ((double)counter_left + counter_right)/2;
//     printf("Average value of counter %g",count_average);
//     distance_final = (count_average * 20.3)/20;
//     printf("Final distance is: %g",distance_final);

// }

// void http_netconn_serve(struct netconn *conn) {
//     const static char http_html_hdr[] = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";
//     const static char http_index_hml[] = "<html><head><title>Encoder distance</title></head><body></body></html>";
//     char dist[5];   // char array for storing distance

//     netconn_write(conn, http_html_hdr, sizeof(http_html_hdr)-1, NETCONN_NOCOPY);
//     netconn_write(conn, http_index_hml, sizeof(http_index_hml)-1, NETCONN_NOCOPY);
//     netconn_write(conn, &counter_left, 4, NETCONN_NOCOPY);
//     netconn_close(conn);
// }

// void http_server()
// {
//     struct netconn *conn, *newconn;
//     err_t err;
//     conn = netconn_new(NETCONN_TCP);
//     netconn_bind(conn, NULL, 80);
//     netconn_listen(conn);
//     do {
//         err = netconn_accept(conn, &newconn);   
//         if(err == ERR_OK) {
//             http_netconn_serve(newconn); //Serve distance
//             netconn_delete(newconn);
//         }
//     } while(err == ERR_OK);
//     netconn_close(conn);
//     netconn_delete(conn);
// }


// void distance_count_left(void *arg)
// {
// 	/*
// 		Set the The LED Pins : GPIO 0 and GPIO 5 to OUTPUT
// 	*/
     
//     gpio_set_direction(left,GPIO_MODE_INPUT);
//     //gpio_set_direction(right,GPIO_MODE_INPUT);
// 	//gpio_set_direction(LED_1,GPIO_MODE_OUTPUT);
// 	//gpio_set_direction(LED_2,GPIO_MODE_OUTPUT);	

// 	while(1)
// 	{
//         while(gpio_get_level(left) == 0)
//         {   
	
//             while(gpio_get_level(left) == 1)
//             {
//                 counter_left++;
// 		        //gpio_set_level(LED_1,0);	//Set LED1 ON
// 		        //gpio_set_level(LED_2,0);	//Set LED2 ON
//                 printf("count for left is:");
//                 printf("%d\n", counter_left);
//                 //vTaskDelay(100/portTICK_PERIOD_MS);
// 				break;
//             }
//         }
        

// 		// http_server();
// 	}
	
// }

// void distance_count_right(void *arg)
// {
// 	/*
// 		Set the The LED Pins : GPIO 0 and GPIO 5 to OUTPUT
// 	*/
     
//     //gpio_set_direction(left,GPIO_MODE_INPUT);
//     gpio_set_direction(right,GPIO_MODE_INPUT);
// 	//gpio_set_direction(LED_1,GPIO_MODE_OUTPUT);
// 	//gpio_set_direction(LED_2,GPIO_MODE_OUTPUT);	

// 	while(1)
// 	{	
// 		if(counter_right>=40)
// 		{
// 			vTaskDelay(10000);
// 		}
//         while(gpio_get_level(right) == 0)
//         {   

//             while(gpio_get_level(right) == 1)
//             {
//                 counter_right++;
// 		        //gpio_set_level(LED_1,0);	//Set LED1 ON
// 		        //gpio_set_level(LED_2,0);	//Set LED2 ON
//                 printf("count for right is:");
//                 printf("%d\n", counter_right);
//                 //vTaskDelay(100/portTICK_PERIOD_MS);
// 				break;
//             }
//         }

// 		// http_server();
// 	}
	
// }

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
	vTaskDelay(1000/portTICK_PERIOD_MS);    // Don't start immediately on bootup, because it's inconvenient -_-
    
 	prev_error = 0; // Added as a precautionary measure

     // Initialize motor control
 	mcpwm_initialize();

     // Tuning using WiFi functions
 	 //nvs_flash_init();
 	// initialise_wifi();
	gpio_set_direction(LED_0, GPIO_MODE_OUTPUT);
	gpio_set_direction(LED_1, GPIO_MODE_OUTPUT);
	gpio_set_level(LED_0, 1);
	gpio_set_level(LED_1, 1);
 	xTaskCreatePinnedToCore(&line_follow,"line following",4096,NULL,1,NULL,0);


    //xTaskCreatePinnedToCore(&distance_count_left,"distance_count_left",4096,NULL,1,NULL,0);
	//xTaskCreatePinnedToCore(&distance_count_right,"distance_count_right",4096,NULL,1,NULL,1);
	
}