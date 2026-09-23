#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

//---------------------------------------------------
#define QUEUE_LENGTH 5

//---------------------------------------------------
static StaticQueue_t queueControlBlock;

static uint8_t queueStorage[
    QUEUE_LENGTH * sizeof(int)
];

//---------------------------------------------------
static QueueHandle_t staticQueue;

//---------------------------------------------------
static void producerTask(void *pvParameters)
{
    int value = 0;

    while (1)
    {
        value++;

        if (xQueueSend(staticQueue, &value, portMAX_DELAY) == pdPASS)
        {
            printf("Producer: Sent %d\n", value);
        }

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
                staticQueue,
                &receivedValue,
                portMAX_DELAY) == pdPASS)
        {
            printf("Consumer: Received %d\n", receivedValue);
        }
    }
}

//---------------------------------------------------
void app_main(void)
{
    //---------------------------------------------------
    staticQueue = xQueueCreateStatic(
        QUEUE_LENGTH,
        sizeof(int),
        queueStorage,
        &queueControlBlock
    );

    //---------------------------------------------------
    if (staticQueue == NULL)
    {
        printf("Static Queue creation failed\n");

        return;
    }

    printf("Static Queue created successfully\n");

    //---------------------------------------------------
    xTaskCreate(
        producerTask,
        "ProducerTask",
        2048,
        NULL,
        1,
        NULL
    );

    //---------------------------------------------------
    xTaskCreate(
        consumerTask,
        "ConsumerTask",
        2048,
        NULL,
        1,
        NULL
    );
}



