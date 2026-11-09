
//main.c

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "my_esp32_board.h"
#include "light_driver.h"

#include "driver/ledc.h"
#include "nvs_flash.h"

static const char *TAG = "light_driver_application";

static esp_err_t app_driver_init(void)
  {

    // Initialize NVS
    //https://github.com/espressif/esp-idf/blob/v6.1/examples/storage/nvs/nvs_rw_value/main/nvs_value_example_main.c
   esp_err_t err = nvs_flash_init();
   if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
       // NVS partition was truncated and needs to be erased
       // Retry nvs_flash_init
       ESP_ERROR_CHECK(nvs_flash_erase());
       err = nvs_flash_init();
   }
   ESP_ERROR_CHECK(err);






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

    //////////////////////


    /*
    ESP_ERROR_CHECK(light_driver_update_rgb(0, 255, 0));
    vTaskDelay(pdMS_TO_TICKS(2000));  // Still dark

    ESP_ERROR_CHECK(light_driver_set_switch(true));
    vTaskDelay(pdMS_TO_TICKS(2000));  // Green ie the hardcoded colour

    ESP_ERROR_CHECK(light_driver_update_rgb(255, 0, 128));
    vTaskDelay(pdMS_TO_TICKS(2000));  // User chosen colour of Purple

    ESP_ERROR_CHECK(light_driver_set_switch(false));
    vTaskDelay(pdMS_TO_TICKS(2000));  // Dark

    ESP_ERROR_CHECK(light_driver_set_switch(true));  // Purple again
    */


    //testing
    ESP_ERROR_CHECK(light_driver_set_switch(true));  //turn light on with default hardcoded colour
    vTaskDelay(pdMS_TO_TICKS(5000));
    //Choose another colour, save and select it
    ESP_LOGI(TAG, "Setting and saving red");
    light_driver_update_rgb(255, 0, 0); //update line with colour = red. issue = this is reporting light_driver_set_switch from hardcoded: red=255 green=0 blue=0
    ESP_LOGI(TAG, "red update succeeded");
    /*
    1. Flash board with update line active so value is saved and new colour displayed

    2. Flash board with update line commented out

    This way we can see if the orange colour comes via NVS and not just the update line. ie dont want to update automatically at every boot


    */
    //saved color shoud be displayed





    return ESP_OK;

  }

void app_main()
{
    ESP_LOGI(TAG, "Application driver initialization");
    ESP_ERROR_CHECK(app_driver_init());
}
