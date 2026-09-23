#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "driver/gpio.h"

//---------------------------------------------------

#define BUTTON_GPIO    GPIO_NUM_4
#define LED_GPIO       GPIO_NUM_2

//---------------------------------------------------

static SemaphoreHandle_t gpioSemaphore = NULL;

//---------------------------------------------------

static void IRAM_ATTR buttonISRHandler(void *arg)
{
    BaseType_t higherPriorityTaskWoken = pdFALSE;

    xSemaphoreGiveFromISR(
        gpioSemaphore,
        &higherPriorityTaskWoken
    );

    if (higherPriorityTaskWoken == pdTRUE)
    {
        portYIELD_FROM_ISR();
    }
}

//---------------------------------------------------

static void buttonTask(void *pvParameters)
{
    while (1)
    {
        if (xSemaphoreTake(
                gpioSemaphore,
                portMAX_DELAY) == pdTRUE)
        {
            printf("GPIO interrupt received\n");

            gpio_set_level(LED_GPIO, 1);

            vTaskDelay(pdMS_TO_TICKS(200));

            gpio_set_level(LED_GPIO, 0);
        }
    }
}

//---------------------------------------------------

void app_main(void)
{
    //---------------------------------------------------
    // Create binary semaphore
    //---------------------------------------------------

    gpioSemaphore = xSemaphoreCreateBinary();

    if (gpioSemaphore == NULL)
    {
        printf("Failed to create semaphore\n");
        return;
    }

    //---------------------------------------------------
    // Configure LED GPIO
    //---------------------------------------------------

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
    // Configure Button GPIO
    //---------------------------------------------------

    gpio_config_t buttonConfig =
    {
        .pin_bit_mask = (1ULL << BUTTON_GPIO),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE
    };

    gpio_config(&buttonConfig);

    //---------------------------------------------------
    // Create GPIO processing task
    //---------------------------------------------------

    xTaskCreate(
        buttonTask,
        "ButtonTask",
        4096,
        NULL,
        2,
        NULL
    );

    //---------------------------------------------------
    // Install GPIO ISR service
    //---------------------------------------------------

    gpio_install_isr_service(0);

    //---------------------------------------------------
    // Attach ISR
    //---------------------------------------------------

    gpio_isr_handler_add(
        BUTTON_GPIO,
        buttonISRHandler,
        NULL
    );

    //---------------------------------------------------

    printf("GPIO ISR Semaphore system started\n");
    printf("Button GPIO : %d\n", BUTTON_GPIO);
    printf("LED GPIO    : %d\n", LED_GPIO);

    //---------------------------------------------------

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------



