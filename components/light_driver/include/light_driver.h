

//light_driver.h

#ifndef LIGHT_DRIVER_H
#define LIGHT_DRIVER_H

#ifdef  __cplusplus
extern "C" {
#endif

#include "iot_driver.h"
#include <stdbool.h>


typedef struct {  //user defined colour, 8-bit RGB values (0–255)
    uint8_t red_value;
    uint8_t green_value;
    uint8_t blue_value;
} rgb_colour;

//rgb_colour user_colour =  {0 ,0, 255};  //red. //causes a compilation error if defined here here, put in light_driver.c instead






typedef struct {  //Light driver init arg type
    gpio_num_t gpio_red;      /**< Red corresponds to GPIO */
    gpio_num_t gpio_green;    /**< Green corresponds to GPIO */
    gpio_num_t gpio_blue;     /**< Blue corresponds to GPIO */
    uint32_t freq_hz;         /**< LEDC timer frequency (Hz) */
    ledc_clk_cfg_t clk_cfg;   /**< Clock srouce of LEDC */
    ledc_timer_bit_t duty_resolution;  /**< LEDC channel duty resolution */
} light_driver_config_t;

esp_err_t light_driver_init(light_driver_config_t *config);
esp_err_t light_driver_set_switch(bool light_status); //	Turn light on/off

esp_err_t light_driver_set_rgb(uint8_t new_red, uint8_t new_green, uint8_t new_blue);


#ifdef __cplusplus
}
#endif

#endif/**< LIGHT_DRIVER_H */
