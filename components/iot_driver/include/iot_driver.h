
//iot_driver.h

/*
Setting up a channel of the LEDC in either high or low speed mode is done in three steps:
1. Timer Configuration by specifying the PWM signal's frequency and duty cycle resolution.
2. Channel Configuration by associating it with the timer and GPIO to output the PWM signal.
3. Change PWM Signal that drives the output in order to change LED's intensity. This can be done under the full control of software or with hardware fading functions.
*/

/*
In the original code, the separate hardware timer is used to manage fading in and out.

But there are two different types of timer involved:
1. LEDC timer — generates the PWM timing (5 kHz in your case). Essential for your RGB driver.
2. General-purpose hardware timer — periodically updates the PWM duty cycle to produce gradual fading. Used by the book's original implementation.
For example, fading from 0% to 100% brightness:
- The LEDC timer continuously generates PWM.
- The general-purpose timer triggers updates every 20 ms.
- Each update increases the duty cycle until it reaches 100%.
For your minimal driver, remove the general-purpose hardware timer entirely. Keep the LEDC timer.

You can add fading later using ESP-IDF's built-in LEDC hardware fade functionality, without needing your own general-purpose timer.

*/

/*
For your minimal ESP32 RGB driver, the sequence is:

1.	iot_led_init()	Configure the LEDC timer (frequency, resolution)
2.	iot_led_regist_channel()	Register each RGB channel, assign GPIO and timer
3.	light_driver_set_rgb()	Set desired red, green and blue brightness
4.	light_driver_set_switch(true)	Switch the light on
5.	light_driver_set_switch(false)	Switch the light off
*/

#ifndef IOT_DRIVER_H
#define IOT_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "driver/ledc.h"  //just for the types of variables declared in this file. https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/ledc.html

esp_err_t iot_led_init(ledc_timer_t timer_num, ledc_mode_t speed_mode, uint32_t freq_hz, ledc_clk_cfg_t clk_cfg, ledc_timer_bit_t duty_resolution); //Configure the LEDC timer (frequency, resolution)
esp_err_t iot_led_regist_channel(ledc_channel_t channel, gpio_num_t gpio_num, ledc_mode_t speed_mode, ledc_timer_t timer_num); //Register each RGB channel, assign GPIO and timer
esp_err_t iot_led_set_channel(ledc_mode_t speed_mode, ledc_channel_t channel, uint8_t rgb_colour); //set duty ie brightness when required ie when changing brightness or switching on. this now also converts from user defined RGB to duty

#ifdef __cplusplus
}
#endif

#endif
