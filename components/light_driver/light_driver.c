
//light_driver.c

#include <stdio.h>
#include <string.h>

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

#include "light_driver.h"
#include <stdint.h>
//#include "app_storage.h" //no storage for minimal driver

//channel layout of 3 colour led light
enum light_channel {
    CHANNEL_ID_RED = 0,
    CHANNEL_ID_GREEN,
    CHANNEL_ID_BLUE,
};

static const char *TAG = "light_driver";

//test by editing user_colour directly
//static rgb_colour user_colour = {0, 0, 255}; //blue
//static rgb_colour user_colour = {255, 0, 0}; //red
//static rgb_colour user_colour = {0, 255, 0}; // Green
static rgb_colour user_colour = {255, 0, 128}; // purple

static bool light_is_on = false;

esp_err_t light_driver_init(light_driver_config_t *config) {


  if (config == NULL) {
      return ESP_ERR_INVALID_ARG;
  }

    esp_err_t ret = iot_led_init(LEDC_TIMER_0, LEDC_LOW_SPEED_MODE, config->freq_hz, config->clk_cfg, config->duty_resolution); //ret defined here

    if (ret != ESP_OK) {
       return ret;
   }

    ret = iot_led_regist_channel(CHANNEL_ID_RED, config->gpio_red,LEDC_LOW_SPEED_MODE, LEDC_TIMER_0); //just reuse ret

    if (ret != ESP_OK) {
        return ret;
    }


    ret = iot_led_regist_channel(CHANNEL_ID_GREEN, config->gpio_green,LEDC_LOW_SPEED_MODE, LEDC_TIMER_0); //just reuse ret

    if (ret != ESP_OK) {
        return ret;
    }


    return iot_led_regist_channel(CHANNEL_ID_BLUE, config->gpio_blue, LEDC_LOW_SPEED_MODE, LEDC_TIMER_0); //The final return handles both outcomes. If blue succeeds, the function returns ESP_OK. If blue fails, it passes that error back to the caller.


}




esp_err_t light_driver_set_rgb(uint8_t new_red, uint8_t new_green, uint8_t new_blue) //go via this to change the colour of the light when program is running
    {

    //need to update the program user colours to the new ones chosen by the user ie this method is just an updater
    user_colour.red_value = new_red;
    user_colour.green_value = new_green;
    user_colour.blue_value = new_blue;

    if (light_is_on) {
      return light_driver_set_switch(true);
    }

    return ESP_OK;

    }







esp_err_t light_driver_set_switch(bool on)
{
    uint8_t channel_1_rgb;
    uint8_t channel_2_rgb;
    uint8_t channel_3_rgb;

    //convert the bool to equivalent duty
    switch (on) {
    case false:
        channel_1_rgb = channel_2_rgb = channel_3_rgb = 0;  //don't destroy the state of the user chosen colour just because the light is off
        break;

    case true:
        channel_1_rgb = user_colour.red_value;
        channel_2_rgb = user_colour.green_value;
        channel_3_rgb = user_colour.blue_value;
        break;
    }

    //setting channels

    esp_err_t ret = iot_led_set_channel(
        LEDC_LOW_SPEED_MODE, CHANNEL_ID_RED, channel_1_rgb);
    if (ret != ESP_OK) {
        return ret;
    }

    ret = iot_led_set_channel(
        LEDC_LOW_SPEED_MODE, CHANNEL_ID_GREEN, channel_2_rgb);
    if (ret != ESP_OK) {
        return ret;
    }

    ret = iot_led_set_channel(
        LEDC_LOW_SPEED_MODE, CHANNEL_ID_BLUE, channel_3_rgb);
    if (ret != ESP_OK) {
        return ret;
    }


    light_is_on = on; //now we can use method which updates colour while running

    return ESP_OK;
}
