#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED_GPIO 2  // Change this if your onboard LED is connected to a different GPIO pin

void app_main(void)
{
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);

    while (1) {
        gpio_set_level(LED_GPIO, 1);  // Turn the LED on
        vTaskDelay(1000 / portTICK_PERIOD_MS);  // Wait for 1 second
        gpio_set_level(LED_GPIO, 0);  // Turn the LED off
        vTaskDelay(1000 / portTICK_PERIOD_MS);  // Wait for 1 second
    }
}
