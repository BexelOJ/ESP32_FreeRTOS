#include <stdio.h>
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED_GPIO_PIN 2 // Most ESP32 boards have an onboard LED connected to GPIO2, but check your board

void app_main() 
{
    // Configure the LED GPIO pin as output
    gpio_set_direction(LED_GPIO_PIN, GPIO_MODE_OUTPUT);

    while (1)
      {
        // Turn the LED on
        gpio_set_level(LED_GPIO_PIN, 1);
        vTaskDelay(50/ portTICK_PERIOD_MS); // Wait for 1 second

        // Turn the LED off
        gpio_set_level(LED_GPIO_PIN, 0);
        vTaskDelay(2000 / portTICK_PERIOD_MS); // Wait for 1 second
    }
}


