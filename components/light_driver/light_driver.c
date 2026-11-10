
//light_driver.c

#include <stdio.h>
#include <string.h>

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

#include "light_driver.h"
#include <stdint.h>

#include "nvs.h"

enum light_channel {  //channel layout of 3 colour led light
    CHANNEL_ID_RED = 0,
    CHANNEL_ID_GREEN,
    CHANNEL_ID_BLUE,
};

static const char *TAG = "light_driver";

typedef struct {
    rgb_colour user_colour;
    bool light_is_on;
} light_state_t;

static light_state_t light_state = {
    .user_colour = {255, 0, 128}, // purple
    .light_is_on = false,
};

static esp_err_t load_rgb(void)
{
    nvs_handle_t handle;
    esp_err_t err = nvs_open("light", NVS_READONLY, &handle);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        return ESP_OK;
    }
    if (err != ESP_OK) {
        return err;
    }

    uint32_t packed_rgb;
    err = nvs_get_u32(handle, "rgb", &packed_rgb);
    nvs_close(handle);

    if (err == ESP_ERR_NVS_NOT_FOUND) {
        return ESP_OK;
    }
    if (err != ESP_OK) {
        return err;
    }

    light_state.user_colour.red_value   = (uint8_t)((packed_rgb >> 24) & 0xFFu);
    light_state.user_colour.green_value = (uint8_t)((packed_rgb >> 16) & 0xFFu);
    light_state.user_colour.blue_value  = (uint8_t)((packed_rgb >>  8) & 0xFFu);

    /*
    packed_rgb >> 24 shifts red three bytes to the right. For purple, FF 00 80 00 becomes 00 00 00 FF
    & 0xFFu keeps only the rightmost eight bits. For red, the shift has already cleared the other bytes. The mask is useful for green and blue because bytes can still be sitting to their left.
    uint8_t) converts the result to the one-byte type your colour field uses. Here, that’s 255.
    */

    return ESP_OK;
}

static esp_err_t save_rgb(uint32_t packed_rgb)  //Packed means putting three separate RGB values into one integer. Each colour fits in 8 bits, so you can fit all three into a uint32_t and save them under one NVS key.
{
    nvs_handle_t handle;
    esp_err_t err = nvs_open("light", NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        return err;
    }

    err = nvs_set_u32(handle, "rgb", packed_rgb);
    if (err == ESP_OK) {
        err = nvs_commit(handle);
    }

    nvs_close(handle);
    return err;
}

esp_err_t light_driver_init(light_driver_config_t *config) {

  if (config == NULL) {
      return ESP_ERR_INVALID_ARG;
  }


  esp_err_t ret = load_rgb();
  if (ret != ESP_OK) {
  return ret;
  }


  ret = iot_led_init(LEDC_TIMER_0, LEDC_LOW_SPEED_MODE, config->freq_hz, config->clk_cfg, config->duty_resolution);

  if (ret != ESP_OK) {
     return ret;
 }

  ret = iot_led_regist_channel(CHANNEL_ID_RED, config->gpio_red,LEDC_LOW_SPEED_MODE, LEDC_TIMER_0);

  if (ret != ESP_OK) {
      return ret;
  }


  ret = iot_led_regist_channel(CHANNEL_ID_GREEN, config->gpio_green,LEDC_LOW_SPEED_MODE, LEDC_TIMER_0); //just reuse ret

  if (ret != ESP_OK) {
      return ret;
  }


  return iot_led_regist_channel(CHANNEL_ID_BLUE, config->gpio_blue, LEDC_LOW_SPEED_MODE, LEDC_TIMER_0); //The final return handles both outcomes. If blue succeeds, the function returns ESP_OK. If blue fails, it passes that error back to the caller.

}


esp_err_t light_driver_update_rgb(uint8_t new_red, uint8_t new_green, uint8_t new_blue) //use this to chaneg light colour when program is running
    {

    uint32_t packed_rgb = ((uint32_t)new_red   << 24) | ((uint32_t)new_green << 16) | ((uint32_t)new_blue  <<  8);  //pack new colour values for saving

    //don't want to change to the new colour if it can't be saved
    //save to flash
    esp_err_t err = save_rgb(packed_rgb);
    if (err != ESP_OK) {

        return err; //If saving fails, return the error and keep the previous colour in RAM and on the LED.
    }

    //now the new colour is saved we can update current light colour
    light_state.user_colour.red_value = new_red;
    light_state.user_colour.green_value = new_green;
    light_state.user_colour.blue_value = new_blue;

    // update the LED hardware
    if (light_state.light_is_on) {
      return light_driver_set_switch(true);
    }

    return ESP_OK;

    }

//this function just uses the hardcoded colour
//a seperate function is called at startup (and whenever needed) to get a saved colour
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

        channel_1_rgb = light_state.user_colour.red_value;
        channel_2_rgb = light_state.user_colour.green_value;
        channel_3_rgb = light_state.user_colour.blue_value;
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

    light_state.light_is_on = on; //now we are permitted to call light_driver_update_rgb which updates colour while running

    return ESP_OK;
}
