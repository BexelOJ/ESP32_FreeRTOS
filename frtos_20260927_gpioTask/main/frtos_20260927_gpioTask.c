#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"

//---------------------------------------------------

#define LED_GPIO        GPIO_NUM_2
#define BUTTON_GPIO     GPIO_NUM_0

#define TASK_PRIORITY   5
#define TASK_STACK_SIZE 4096

//---------------------------------------------------

static void gpioTask(void *pvParameters)
{
    int previousButtonState = 1;

    while (1)
    {
        int buttonState = gpio_get_level(BUTTON_GPIO);

        if (buttonState != previousButtonState)
        {
            if (buttonState == 0)
            {
                printf(
                    "Button pressed on Core %d\n",
                    xPortGetCoreID()
                );

                gpio_set_level(LED_GPIO, 1);
            }
            else
            {
                printf("Button released\n");

                gpio_set_level(LED_GPIO, 0);
            }

            previousButtonState = buttonState;
        }

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

//---------------------------------------------------

void app_main(void)
{
    gpio_config_t ledConfig =
    {
        .pin_bit_mask = (1ULL << LED_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&ledConfig);

    //---------------------------------------------------

    gpio_config_t buttonConfig =
    {
        .pin_bit_mask = (1ULL << BUTTON_GPIO),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&buttonConfig);

    //---------------------------------------------------

    xTaskCreate(
        gpioTask,
        "GPIOTask",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        NULL
    );

    printf("GPIO task demonstration started\n");

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------



