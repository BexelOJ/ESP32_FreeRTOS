#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define BLINK_GPIO  2   // built‑in LED on most ESP32 DevKit boards

void blink_task(void *pvParameter)
{
    int gpio = *(int*)pvParameter;

    gpio_set_direction(gpio, GPIO_MODE_OUTPUT);

    while(1) {
        gpio_set_level(gpio, 1);
        vTaskDelay(pdMS_TO_TICKS(1000));

        gpio_set_level(gpio, 0);
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void app_main(void)
{
    int gpio = BLINK_GPIO;

    xTaskCreate(&blink_task, "blink", 2048, &gpio, 5, NULL);
}

