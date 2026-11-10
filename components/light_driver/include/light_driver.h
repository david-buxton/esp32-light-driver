
//light_driver.h

#ifndef LIGHT_DRIVER_H
#define LIGHT_DRIVER_H

#ifdef  __cplusplus
extern "C" {
#endif

#include "iot_driver.h"
//REQUIRES vs PROV_REQUIRES
//do callers including this file (light_driver.h) need to find iot_driver.h hence use REQUIRES
//ie main.c needs to be able to find iot_driver.h even though it only names light_driver.h
//ie it just it just facilitates a clean dependency chain, would not want main including iot led headers directly
#include <stdbool.h>

typedef struct {
    uint8_t red_value;
    uint8_t green_value;
    uint8_t blue_value;
} rgb_colour; //user defined colour, 8-bit RGB values (0–255)

typedef struct {
    gpio_num_t gpio_red;
    gpio_num_t gpio_green;
    gpio_num_t gpio_blue;
    uint32_t freq_hz;         /**< LEDC timer frequency (Hz) */
    ledc_clk_cfg_t clk_cfg;   /**< Clock source of LEDC */
    ledc_timer_bit_t duty_resolution;  /**< LEDC channel duty resolution */
} light_driver_config_t;

esp_err_t light_driver_init(light_driver_config_t *config);
esp_err_t light_driver_set_switch(bool light_status); //	Turn light on/off
esp_err_t light_driver_update_rgb(uint8_t new_red, uint8_t new_green, uint8_t new_blue); //use this to change light colour when program is running and save new values to flash

#ifdef __cplusplus
}
#endif

#endif
