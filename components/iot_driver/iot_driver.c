
//iot_driver.c

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "errno.h"

#include "math.h"
#include "esp_log.h"
#include "soc/ledc_reg.h"
#include "soc/timer_group_struct.h"
#include "soc/ledc_struct.h"
#include "driver/timer.h"
#include "driver/ledc.h"
#include "iot_driver.h"

esp_err_t iot_led_init(ledc_timer_t timer_num,
                      ledc_mode_t speed_mode,
                      uint32_t freq_hz,
                      ledc_clk_cfg_t clk_cfg,
                      ledc_timer_bit_t duty_resolution)
{
    const ledc_timer_config_t ledc_time_config = {
        .speed_mode      = speed_mode,
        .duty_resolution = duty_resolution,
        .timer_num       = timer_num,
        .freq_hz         = freq_hz,
        .clk_cfg         = clk_cfg,
    };

    return ledc_timer_config(&ledc_time_config);

}

esp_err_t iot_led_regist_channel(ledc_channel_t channel,
                                gpio_num_t gpio_num,
                                ledc_mode_t speed_mode,
                                ledc_timer_t timer_num) //called for each channel
{
    const ledc_channel_config_t ledc_ch_config = {  //building the channel configuration structure
        .gpio_num   = gpio_num,
        .channel    = channel,
        .speed_mode = speed_mode,
        .timer_sel  = timer_num,
        .flags.output_invert = 0,
    };

    return ledc_channel_config(&ledc_ch_config);

}

esp_err_t iot_led_set_channel(
    ledc_mode_t speed_mode,
    ledc_channel_t channel,
    uint8_t rgb_colour)

    {

    uint32_t duty = ((uint32_t)rgb_colour * 2047u + 127u) / 255u;  //we use uint32_t widening here, adding half the divisor before the actual division makes the result round to the nearest whole duty instead of always rounding down.

    esp_err_t ret = ledc_set_duty(speed_mode, channel, duty);

    if (ret != ESP_OK) {
        return ret;
    }

        return ledc_update_duty(speed_mode, channel);
    }
