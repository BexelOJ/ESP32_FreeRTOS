#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

//---------------------------------------------------
#define MAX_COUNT 5

//---------------------------------------------------
static SemaphoreHandle_t countingSemaphore = NULL;

//---------------------------------------------------
static void producerTask(void *pvParameters)
{
    int eventNumber = 1;

    while (1)
    {
        printf("Producer: Event %d occurred\n", eventNumber++);

        if (xSemaphoreGive(countingSemaphore) == pdTRUE)
        {
            printf("Producer: Counting semaphore incremented\n");
        }
        else
        {
            printf("Producer: Semaphore count is already at maximum\n");
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------
static void consumerTask(void *pvParameters)
{
    while (1)
    {
        printf("Consumer: Waiting for event...\n");

        if (xSemaphoreTake(countingSemaphore, portMAX_DELAY) == pdTRUE)
        {
            printf("Consumer: Event consumed\n");
        }

        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}

//---------------------------------------------------
void app_main(void)
{
    countingSemaphore = xSemaphoreCreateCounting(MAX_COUNT, 0);

    if (countingSemaphore == NULL)
    {
        printf("Failed to create counting semaphore\n");
        return;
    }

    printf("Counting semaphore created\n");
    printf("Maximum count: %d\n", MAX_COUNT);

    //---------------------------------------------------
    xTaskCreatePinnedToCore(
        producerTask,
        "ProducerTask",
        2048,
        NULL,
        1,
        NULL,
        0
    );

    //---------------------------------------------------
    xTaskCreatePinnedToCore(
        consumerTask,
        "ConsumerTask",
        2048,
        NULL,
        1,
        NULL,
        1
    );
}



