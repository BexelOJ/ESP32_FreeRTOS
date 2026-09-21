#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

//---------------------------------------------------

#define QUEUE_LENGTH    5
#define QUEUE_ITEM_SIZE sizeof(int)

//---------------------------------------------------

static QueueHandle_t taskQueue;

//---------------------------------------------------

static void producerTask(void *pvParameters)
{
    int value = 0;

    while (1)
    {
        value++;

        xQueueSend(taskQueue, &value, portMAX_DELAY);

        printf("Producer: sent %d\n", value);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------

static void consumerTask(void *pvParameters)
{
    int receivedValue;

    while (1)
    {
        if (xQueueReceive(taskQueue, &receivedValue, portMAX_DELAY) == pdTRUE)
        {
            printf("Consumer: received %d\n", receivedValue);
        }
    }
}

//---------------------------------------------------

void app_main(void)
{
    taskQueue = xQueueCreate(QUEUE_LENGTH, QUEUE_ITEM_SIZE);

    if (taskQueue == NULL)
    {
        printf("Failed to create queue\n");
        return;
    }

    xTaskCreatePinnedToCore(
        producerTask,
        "Producer Task",
        2048,
        NULL,
        1,
        NULL,
        0
    );

    xTaskCreatePinnedToCore(
        consumerTask,
        "Consumer Task",
        2048,
        NULL,
        1,
        NULL,
        1
    );
}


//---------------------------------------------------

