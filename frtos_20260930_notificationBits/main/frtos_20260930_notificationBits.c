#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//---------------------------------------------------

#define TASK_PRIORITY      5
#define TASK_STACK_SIZE    4096

//---------------------------------------------------

#define EVENT_SENSOR       (1UL << 0)
#define EVENT_NETWORK      (1UL << 1)
#define EVENT_BUTTON       (1UL << 2)
#define EVENT_ERROR        (1UL << 3)

//---------------------------------------------------

static TaskHandle_t eventTaskHandle = NULL;

//---------------------------------------------------

static void eventProducerTask(void *pvParameters)
{
    while (1)
    {
        printf("Producer: SENSOR event\n");

        xTaskNotify(
            eventTaskHandle,
            EVENT_SENSOR,
            eSetBits
        );

        vTaskDelay(pdMS_TO_TICKS(1000));

        printf("Producer: NETWORK event\n");

        xTaskNotify(
            eventTaskHandle,
            EVENT_NETWORK,
            eSetBits
        );

        vTaskDelay(pdMS_TO_TICKS(1000));

        printf("Producer: BUTTON event\n");

        xTaskNotify(
            eventTaskHandle,
            EVENT_BUTTON,
            eSetBits
        );

        vTaskDelay(pdMS_TO_TICKS(1000));

        printf("Producer: ERROR event\n");

        xTaskNotify(
            eventTaskHandle,
            EVENT_ERROR,
            eSetBits
        );

        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}

//---------------------------------------------------

static void eventTask(void *pvParameters)
{
    uint32_t notificationValue;

    while (1)
    {
        xTaskNotifyWait(
            0,
            UINT32_MAX,
            &notificationValue,
            portMAX_DELAY
        );

        printf(
            "\nEventTask received notification: 0x%08lX\n",
            (unsigned long)notificationValue
        );

        if (notificationValue & EVENT_SENSOR)
        {
            printf("EventTask: SENSOR event\n");
        }

        if (notificationValue & EVENT_NETWORK)
        {
            printf("EventTask: NETWORK event\n");
        }

        if (notificationValue & EVENT_BUTTON)
        {
            printf("EventTask: BUTTON event\n");
        }

        if (notificationValue & EVENT_ERROR)
        {
            printf("EventTask: ERROR event\n");
        }

        printf("\n");
    }
}

//---------------------------------------------------

void app_main(void)
{
    xTaskCreate(
        eventTask,
        "EventTask",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        &eventTaskHandle
    );

    xTaskCreate(
        eventProducerTask,
        "EventProducer",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        NULL
    );

    printf("Notification bits demonstration started\n");
}

//---------------------------------------------------



