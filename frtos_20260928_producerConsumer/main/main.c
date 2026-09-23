#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

//---------------------------------------------------

#define PRODUCER_PRIORITY      5
#define CONSUMER_PRIORITY      5

#define TASK_STACK_SIZE        4096
#define QUEUE_LENGTH           5

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
            "Producer: Core %d | "
            "Producing value = %d\n",
            xPortGetCoreID(),
            value
        );

        if (xQueueSend(
                dataQueue,
                &value,
                pdMS_TO_TICKS(100)) != pdPASS)
        {
            printf("Producer: Queue full\n");
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------

static void consumerTask(void *pvParameters)
{
    int value;

    while (1)
    {
        if (xQueueReceive(
                dataQueue,
                &value,
                portMAX_DELAY) == pdPASS)
        {
            printf(
                "Consumer: Core %d | "
                "Consumed value = %d\n",
                xPortGetCoreID(),
                value
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
        PRODUCER_PRIORITY,
        NULL,
        0
    );

    xTaskCreatePinnedToCore(
        consumerTask,
        "ConsumerTask",
        TASK_STACK_SIZE,
        NULL,
        CONSUMER_PRIORITY,
        NULL,
        1
    );

    printf("Producer-consumer demonstration started\n");

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------



