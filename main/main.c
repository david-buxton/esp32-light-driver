
//main.c

/*
Macro	Level
ESP_LOGE()	Error
ESP_LOGW()	Warning
ESP_LOGI()	Information
ESP_LOGD()	Debug
ESP_LOGV()	Verbose
*/

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "my_esp32_board.h" //to supply app_driver_init()
#include "light_driver.h"

#include "driver/ledc.h" //to supply app_driver_init () with clk_cfg and duty_resolution. driver/ledc.h includes hal/ledc_types.h, making those definitions available to your application.

static const char *TAG = "light_driver_application";

static esp_err_t app_driver_init(void)
  {
    light_driver_config_t driver_config = {
        .gpio_red      = LIGHT_GPIO_RED,  //via board header
        .gpio_green      = LIGHT_GPIO_GREEN,  //via board header
        .gpio_blue       = LIGHT_GPIO_BLUE, //via board header

        .freq_hz         = LIGHT_FREQ_HZ, //via board header, (lower level type of config but i will keep in app level for now)
        .clk_cfg         = LEDC_USE_APB_CLK,  //via ledc_types.hal, (lower level type of config but i will keep in app level for now)
        .duty_resolution = LEDC_TIMER_11_BIT, //via ledc_types.hal, (lower level type of config but i will keep in app level for now)
    };

    ESP_ERROR_CHECK(light_driver_init(&driver_config)); //using the next layer down API
    //light_driver_set_switch(true);

    //light_driver_set_switch(true);
    //vTaskDelay(pdMS_TO_TICKS(2000));

    //light_driver_set_switch(false);
    //vTaskDelay(pdMS_TO_TICKS(2000));

    //light_driver_set_switch(true);

    //ESP_LOGI(TAG, "Light ON");

    ESP_ERROR_CHECK(light_driver_set_rgb(0, 255, 0));
    vTaskDelay(pdMS_TO_TICKS(2000));  // Still dark

    ESP_ERROR_CHECK(light_driver_set_switch(true));
    vTaskDelay(pdMS_TO_TICKS(2000));  // Green

    ESP_ERROR_CHECK(light_driver_set_rgb(255, 0, 128));
    vTaskDelay(pdMS_TO_TICKS(2000));  // Purple

    ESP_ERROR_CHECK(light_driver_set_switch(false));
    vTaskDelay(pdMS_TO_TICKS(2000));  // Dark

    ESP_ERROR_CHECK(light_driver_set_switch(true));  // Purple again

    return ESP_OK;

  }

void app_main()
{
    ESP_LOGI(TAG, "Application driver initialization");
    ESP_ERROR_CHECK(app_driver_init());
}
