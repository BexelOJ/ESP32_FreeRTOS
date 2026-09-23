#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "driver/gpio.h"

//---------------------------------------------------

#define BUTTON_1_GPIO    GPIO_NUM_4
#define BUTTON_2_GPIO    GPIO_NUM_5
#define LED_GPIO         GPIO_NUM_2

//---------------------------------------------------

#define EVENT_BUTTON_1   (1 << 0)
#define EVENT_BUTTON_2   (1 << 1)

//---------------------------------------------------

static EventGroupHandle_t gpioEventGroup = NULL;

//---------------------------------------------------

static void IRAM_ATTR button1ISRHandler(void *arg)
{
    BaseType_t higherPriorityTaskWoken = pdFALSE;

    xEventGroupSetBitsFromISR(
        gpioEventGroup,
        EVENT_BUTTON_1,
        &higherPriorityTaskWoken
    );

    if (higherPriorityTaskWoken == pdTRUE)
    {
        portYIELD_FROM_ISR();
    }
}

//---------------------------------------------------

static void IRAM_ATTR button2ISRHandler(void *arg)
{
    BaseType_t higherPriorityTaskWoken = pdFALSE;

    xEventGroupSetBitsFromISR(
        gpioEventGroup,
        EVENT_BUTTON_2,
        &higherPriorityTaskWoken
    );

    if (higherPriorityTaskWoken == pdTRUE)
    {
        portYIELD_FROM_ISR();
    }
}

//---------------------------------------------------

static void gpioTask(void *pvParameters)
{
    while (1)
    {
        EventBits_t eventBits = xEventGroupWaitBits(
            gpioEventGroup,
            EVENT_BUTTON_1 | EVENT_BUTTON_2,
            pdTRUE,
            pdFALSE,
            portMAX_DELAY
        );

        //---------------------------------------------------
        // Button 1 event
        //---------------------------------------------------

        if (eventBits & EVENT_BUTTON_1)
        {
            printf("Button 1 interrupt received\n");

            gpio_set_level(LED_GPIO, 1);

            vTaskDelay(pdMS_TO_TICKS(200));

            gpio_set_level(LED_GPIO, 0);
        }

        //---------------------------------------------------
        // Button 2 event
        //---------------------------------------------------

        if (eventBits & EVENT_BUTTON_2)
        {
            printf("Button 2 interrupt received\n");

            gpio_set_level(LED_GPIO, 1);

            vTaskDelay(pdMS_TO_TICKS(500));

            gpio_set_level(LED_GPIO, 0);
        }
    }
}

//---------------------------------------------------

void app_main(void)
{
    //---------------------------------------------------
    // Create Event Group
    //---------------------------------------------------

    gpioEventGroup = xEventGroupCreate();

    if (gpioEventGroup == NULL)
    {
        printf("Failed to create Event Group\n");
        return;
    }

    //---------------------------------------------------
    // Configure LED
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
    // Configure Button 1
    //---------------------------------------------------

    gpio_config_t button1Config =
    {
        .pin_bit_mask = (1ULL << BUTTON_1_GPIO),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE
    };

    gpio_config(&button1Config);

    //---------------------------------------------------
    // Configure Button 2
    //---------------------------------------------------

    gpio_config_t button2Config =
    {
        .pin_bit_mask = (1ULL << BUTTON_2_GPIO),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE
    };

    gpio_config(&button2Config);

    //---------------------------------------------------
    // Create GPIO task
    //---------------------------------------------------

    xTaskCreate(
        gpioTask,
        "GpioTask",
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
    // Attach Button 1 ISR
    //---------------------------------------------------

    gpio_isr_handler_add(
        BUTTON_1_GPIO,
        button1ISRHandler,
        NULL
    );

    //---------------------------------------------------
    // Attach Button 2 ISR
    //---------------------------------------------------

    gpio_isr_handler_add(
        BUTTON_2_GPIO,
        button2ISRHandler,
        NULL
    );

    //---------------------------------------------------

    printf("GPIO ISR Event Group system started\n");
    printf("Button 1 GPIO : %d\n", BUTTON_1_GPIO);
    printf("Button 2 GPIO : %d\n", BUTTON_2_GPIO);
    printf("LED GPIO      : %d\n", LED_GPIO);

    //---------------------------------------------------

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------



