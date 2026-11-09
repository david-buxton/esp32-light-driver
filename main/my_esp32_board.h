
//my_esp32_board.h

#ifndef MY_ESP32_BOARD_H
#define MY_ESP32_BOARD_H

#ifdef __cplusplus
extern "C" {
#endif

//Light driver Macro

#define LIGHT_GPIO_RED          21  //https://cdn.shopify.com/s/files/1/1509/1638/files/ESP-32_NodeMCU_Developmentboard_Pinout.pdf?v=1609851295
#define LIGHT_GPIO_GREEN        19
#define LIGHT_GPIO_BLUE         18
#define LIGHT_FREQ_HZ           5000    /**specify the PWM frequency for the shared timer */

#ifdef __cplusplus
}
#endif

#endif
