/*
Copyright (c) 2018, Society of Robotics and Automation, VJTI
This is an example code for line following :
Go to : ..../components/SRA/include/TUNING.h and change EXAMPLE_WIFI_SSID and
EXAMPLE_WIFI_PASS with
your Wifi name and Password.
*/

#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "SRA18.h"
 #include "TUNING.h"  //Uncomment for tuning using WiFi

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
*/
float kp= 0.45;
float ki= 0;
float kd= 0.45;

//...................................
/* Motor value constraints
*/
float opt = 75;
float lower_pwm_constrain = 60;
float higher_pwm_constrain = 83;
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
left_pwm = constrain((opt + correction), lower_pwm_constrain, higher_pwm_constrain);
right_pwm = constrain((opt - correction), lower_pwm_constrain, higher_pwm_constrain);

}

/*
 * Function to use ESP32 WiFi to create server to control PID constants
 * Has PID constants for self-balancing also
 */ 
void http_server(void *arg)
{
    printf("%s\n", "http task");
    struct netconn *conn, *newconn;
    err_t err;
    conn = netconn_new(NETCONN_TCP);
    netconn_bind(conn, NULL, 80);
    netconn_listen(conn);
    do {
     err = netconn_accept(conn, &newconn);
     if (err == ERR_OK) {
       http_server_netconn_serve(newconn,&setpoint,&pitch_kP,&pitch_kD,&pitch_kI,&kp,&kd,&ki, &forward_offset, &forward_buffer);
       netconn_delete(newconn);
     }
    } while(err == ERR_OK);
    netconn_close(conn);
    netconn_delete(conn);
}

/*
 * Function for line following, passed in xTaskCreate() in app_main()
 */ 
void line_follow(void *arg)
{

    vTaskDelay(100/ portTICK_RATE_MS);
    while(1)
    {
        // get the raw readings
        read_sensors();
        // Calculate sensor reading in 0 to 1000
        calc_sensor_values();
        // Calculate error in position w.r.t. line
        calc_error();
        // Calculate the correction using PID formula and implement PID control
        calc_correction();
        // Every iteration will send a signal to the motors to keep moving forward 
        bot_forward(MCPWM_UNIT_0, MCPWM_TIMER_0, left_pwm - 2.6, right_pwm);
        // Print values to monitor for testing
        printf("Error: %f\t",error );
        printf("correction: %f ",correction);
        printf("Left Pwm: %f\t",left_pwm );
        printf("Right Pwm: %f\n",right_pwm );  
    }
}
void app_main()
{
    vTaskDelay(1000/portTICK_PERIOD_MS);    // Don't start immediately on bootup, because it's inconvenient -_-
    
	prev_error = 0; // Added as a precautionary measure

    // Initialize motor control
	mcpwm_initialize();

    // Tuning using WiFi functions
	 //nvs_flash_init();
	// initialise_wifi();

	xTaskCreate(&line_follow,"line following",100000,NULL,1,NULL);
	//xTaskCreate(&http_server,"server",10000,NULL,2,NULL);
}
