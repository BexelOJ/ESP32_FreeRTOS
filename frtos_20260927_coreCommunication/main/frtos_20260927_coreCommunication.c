#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

//---------------------------------------------------

#define TASK_PRIORITY    5
#define TASK_STACK_SIZE  4096
#define QUEUE_LENGTH     5

//---------------------------------------------------

static QueueHandle_t dataQueue;

//---------------------------------------------------

static void producerTask(void *pvParameters)
{
    int value = 0;

    while (1)
    {
        value++;

        printf(
            "Producer: Core %d, sending value = %d\n",
            xPortGetCoreID(),
            value
        );

        xQueueSend(
            dataQueue,
            &value,
            portMAX_DELAY
        );

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------

static void consumerTask(void *pvParameters)
{
    int receivedValue;

    while (1)
    {
        if (xQueueReceive(
                dataQueue,
                &receivedValue,
                portMAX_DELAY) == pdPASS)
        {
            printf(
                "Consumer: Core %d, received value = %d\n",
                xPortGetCoreID(),
                receivedValue
            );
        }
    }
}

//---------------------------------------------------

void app_main(void)
{
    dataQueue = xQueueCreate(
        QUEUE_LENGTH,
        sizeof(int)
    );

    if (dataQueue == NULL)
    {
        printf("Failed to create queue\n");
        return;
    }

    xTaskCreatePinnedToCore(
        producerTask,
        "ProducerTask",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        NULL,
        0
    );

    xTaskCreatePinnedToCore(
        consumerTask,
        "ConsumerTask",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        NULL,
        1
    );

    printf("Core-to-core communication started\n");

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------



