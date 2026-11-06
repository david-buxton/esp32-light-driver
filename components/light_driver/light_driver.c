
#include <stdio.h>
#include <string.h>

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

#include "light_driver.h"
//#include "app_storage.h" //no storage for minimal driver

//channel layout of 3 colour led light
enum light_channel {
    CHANNEL_ID_RED = 0,
    CHANNEL_ID_GREEN,
    CHANNEL_ID_BLUE,
};

static const char *TAG = "light_driver";

esp_err_t light_driver_init(light_driver_config_t *config) {


  if (config == NULL) {
      return ESP_ERR_INVALID_ARG;
  }

    esp_err_t ret = iot_led_init(LEDC_TIMER_0, LEDC_LOW_SPEED_MODE, config->freq_hz, config->clk_cfg, config->duty_resolution);

    if (ret != ESP_OK) {
       return ret;
   }

    return iot_led_regist_channel(CHANNEL_ID_RED, config->gpio_red,LEDC_LOW_SPEED_MODE, LEDC_TIMER_0);     //associates the LEDC channel with a particular LEDC timer.
    //iot_led_regist_channel(CHANNEL_ID_GREEN, config->gpio_green,LEDC_LOW_SPEED_MODE, LEDC_TIMER_0); //only one internal LED for now
    //iot_led_regist_channel(CHANNEL_ID_BLUE, config->gpio_blue,LEDC_LOW_SPEED_MODE, LEDC_TIMER_0);   //only one internal LED for now

}






esp_err_t light_driver_set_switch(bool on)
{
    uint32_t duty = on ? 1024 : 0;//on is 1024, off is zero

    esp_err_t ret = iot_led_set_channel(
        LEDC_LOW_SPEED_MODE, CHANNEL_ID_RED, duty
    );

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set LED channel: %s",
                 esp_err_to_name(ret));
        return ret;
    }

    return ESP_OK;
}
