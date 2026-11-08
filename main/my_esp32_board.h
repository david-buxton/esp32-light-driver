
//my_esp32_board.h

//issue to do for all files but left for now - All driver functions	Return ESP_OK without checking whether ESP-IDF calls succeeded

//my board definitions - az delivery esp32 wroom dev board


/*
There are three useful patterns for calling functions with other functions
1. return other_function(); — simplest when you're just propagating its result.
2. esp_err_t ret = other_function(); — useful when you need to inspect, log or process the result.
3. other_function(); — appropriate when you genuinely don't need its return value, such as calling a void function.
Otherwise you may ignore error from the called function and only report eg sucess
*/


/*

Checking errors:
Layer 3 (iot_driver.c): Check ESP-IDF return values and propagate errors upwards.
Layer 2 (light_driver.c): Check Layer 3's return values, stop if necessary, and propagate errors upwards.
Layer 1 (main.c): Decide what to do about failures, using ESP_ERROR_CHECK() during initialisation.

eg in layer 1 we might detect errors and stop with eg:

if (ret != ESP_OK) {
    return ret;
}

The return value of that functions gets passed back to the caller to deal with.
Higher layers can then decide whether to log the error, retry or abort.

*/

#ifndef MY_ESP32_BOARD_H
#define MY_ESP32_BOARD_H

#ifdef __cplusplus
extern "C" {
#endif

//Light driver Macro

#define LIGHT_GPIO_RED          21  //see board pinout (GPIO 2 = internal LED)
#define LIGHT_GPIO_GREEN        19 //see board pinout
#define LIGHT_GPIO_BLUE         18 //see board pinout
#define LIGHT_FREQ_HZ           5000    /**specify the PWM frequency for the shared timer */






#ifdef __cplusplus
}
#endif

#endif
