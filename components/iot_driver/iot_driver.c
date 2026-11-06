
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "errno.h"

#include "math.h"
#include "esp_log.h"
#include "soc/ledc_reg.h"
#include "soc/timer_group_struct.h"
#include "soc/ledc_struct.h"
#include "driver/timer.h" //esp idf
#include "driver/ledc.h"  //esp idf
#include "iot_driver.h"

/*
later populate with eg g_light_config->timer_num  = timer_num;
And remember:
- config.timer_num — when config is a structure.
- config->timer_num — when config is a pointer to a structure.
The -> operator effectively dereferences the pointer and accesses the member in one operation.
the -> operator is used for both reading and writing members of a struct through a pointer.
*/

esp_err_t iot_led_init(ledc_timer_t timer_num,
                      ledc_mode_t speed_mode,
                      uint32_t freq_hz,
                      ledc_clk_cfg_t clk_cfg,
                      ledc_timer_bit_t duty_resolution)  //for timer configuration
{
    const ledc_timer_config_t ledc_time_config = {
        .speed_mode      = speed_mode,
        .duty_resolution = duty_resolution,
        .timer_num       = timer_num,
        .freq_hz         = freq_hz,
        .clk_cfg         = clk_cfg,
    };

    return ledc_timer_config(&ledc_time_config);      // https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/ledc.html#_CPPv419ledc_channel_configPK21ledc_channel_config_t

}





esp_err_t iot_led_regist_channel(ledc_channel_t channel,
                                gpio_num_t gpio_num,
                                ledc_mode_t speed_mode,
                                ledc_timer_t timer_num) //called for each channel
{
    const ledc_channel_config_t ledc_ch_config = {  //building the channel configuration structure
        .gpio_num   = gpio_num,
        .channel    = channel,
        .speed_mode = speed_mode,     //was: g_light_config->speed_mode,
        .timer_sel  = timer_num,      //was: g_light_config->timer_num,
    };

    return ledc_channel_config(&ledc_ch_config);

}






esp_err_t iot_led_set_channel(
    ledc_mode_t speed_mode,
    ledc_channel_t channel,
    uint32_t duty)


    /*
    This is called for a channel whenever the brightness of a channel needs to change eg at switch on

    With your 11-bit resolution, 1024 represents approximately 50% duty cycle.
    ie 2^11 = 2048

    0 → 0% duty (OFF)
    1024 → 50% duty
    2048 → 100% duty (theoretical maximum)
    Important caveat: On the ESP32, setting duty to 2048 with the maximum supported duty resolution for a
    timer can cause a hardware overflow. The usual safe range is 0 to 2047, and you can use 2047 as effectively 100% brightness in your driver.

    */

    //When changing an LEDC channel's duty cycle, you normally need both functions.
{
    esp_err_t ret = ledc_set_duty(speed_mode, channel, duty);

    if (ret != ESP_OK) {
        return ret;
    }

    return ledc_update_duty(speed_mode, channel);
}
