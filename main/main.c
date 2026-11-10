
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

#include "driver/gpio.h"


#define BUTTON_GPIO GPIO_NUM_23

static const char *TAG = "light_driver_application";

static void run_button_loop(void)
{
    const gpio_config_t button_config = { //GPIO pad configuration. Configure GPIO's Mode,pull-up,PullDown,IntrType
        .pin_bit_mask = 1ULL << BUTTON_GPIO,  //uses a bit mask to select the relevant pin(s) to apply the settings to
        .mode = GPIO_MODE_INPUT,  // Enable reading the pin as logic low or high , GPIO_MODE_INPUT lets pin supply 1/0
        .pull_up_en = GPIO_PULLUP_ENABLE, //For a button input, enable only one out of  pull up or pull down. Choose the resistor that gives you the idle level you want
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&button_config));

    bool light_on = false;
    ESP_ERROR_CHECK(light_driver_set_switch(light_on));

    int stable_level = gpio_get_level(BUTTON_GPIO);

    while (true) {
        int level = gpio_get_level(BUTTON_GPIO);

        if (level != stable_level) {  //a potential press is detected

            vTaskDelay(pdMS_TO_TICKS(30));  // Give the contacts time to settle, then check again.

            if (gpio_get_level(BUTTON_GPIO) == level) {
                stable_level = level;

                if (stable_level == 0) {  //ie btn is grounded ie presses
                    light_on = !light_on; //just flip the light on/off state
                    ESP_ERROR_CHECK(light_driver_set_switch(light_on));
                }
            }
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}



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
    .gpio_red      = LIGHT_GPIO_RED,
    .gpio_green      = LIGHT_GPIO_GREEN,
    .gpio_blue       = LIGHT_GPIO_BLUE,

    .freq_hz         = LIGHT_FREQ_HZ,
    .clk_cfg         = LEDC_USE_APB_CLK,
    .duty_resolution = LEDC_TIMER_11_BIT,
  };

    ESP_ERROR_CHECK(light_driver_init(&driver_config));





    ESP_ERROR_CHECK(light_driver_set_switch(true));  //turn light on with default hardcoded colour
    vTaskDelay(pdMS_TO_TICKS(5000));
    ESP_LOGI(TAG, "Setting and saving red");
    light_driver_update_rgb(255, 0, 0); //update colour line. Comment this in/out as per section below
    ESP_LOGI(TAG, "Red update succeeded");

    /*

    Testing the program

    NB. NVS must be empty before the test run starts - If red is already saved in NVS, the LED will come on red immediately

    1. Flash with update line active

    Expected hardware behaviour:
    The light will turn off when the button loop starts. Purple default for five seconds, then red showing that the default colour is used when NVS is empty and that
    light_driver_update_rgb() changes the LED immediately while it’s on (via RAM)

    2. Flash agin with update line commented out

    Expected hardware behaviour:
    Red as soon as the LED lights showing that the driver restored red from NVS (as restart after flashing resets RAM values).
    light_driver_init() loads red into RAM from NVS, then light_driver_set_switch(true) uses that value.

    */

    return ESP_OK;

  }

void app_main()
{
    ESP_LOGI(TAG, "Application driver initialization");
    ESP_ERROR_CHECK(app_driver_init());
    run_button_loop();
}
