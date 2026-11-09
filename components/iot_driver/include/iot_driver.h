
//iot_driver.h

#ifndef IOT_DRIVER_H
#define IOT_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "driver/ledc.h"

esp_err_t iot_led_init(ledc_timer_t timer_num, ledc_mode_t speed_mode, uint32_t freq_hz, ledc_clk_cfg_t clk_cfg, ledc_timer_bit_t duty_resolution); //Configure the LEDC timer (frequency, resolution)
esp_err_t iot_led_regist_channel(ledc_channel_t channel, gpio_num_t gpio_num, ledc_mode_t speed_mode, ledc_timer_t timer_num); //Register each RGB channel, assign GPIO and timer
esp_err_t iot_led_set_channel(ledc_mode_t speed_mode, ledc_channel_t channel, uint8_t rgb_colour); //set duty ie brightness when required ie when changing brightness or switching on.

#ifdef __cplusplus
}
#endif

#endif
