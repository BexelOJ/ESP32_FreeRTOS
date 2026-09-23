#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

//---------------------------------------------------
static SemaphoreHandle_t mutex = NULL;

//---------------------------------------------------
static int sharedResource = 0;

//---------------------------------------------------
static void taskA(void *pvParameters)
{
    while (1)
    {
        if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE)
        {
            printf("Task A: Mutex acquired\n");

            sharedResource++;

            printf("Task A: Shared resource = %d\n", sharedResource);

            vTaskDelay(pdMS_TO_TICKS(1000));

            printf("Task A: Releasing mutex\n");

            xSemaphoreGive(mutex);
        }

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

//---------------------------------------------------
static void taskB(void *pvParameters)
{
    while (1)
    {
        if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE)
        {
            printf("Task B: Mutex acquired\n");

            sharedResource++;

            printf("Task B: Shared resource = %d\n", sharedResource);

            vTaskDelay(pdMS_TO_TICKS(1000));

            printf("Task B: Releasing mutex\n");

            xSemaphoreGive(mutex);
        }

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

//---------------------------------------------------
void app_main(void)
{
    mutex = xSemaphoreCreateMutex();

    if (mutex == NULL)
    {
        printf("Failed to create mutex\n");
        return;
    }

    printf("Mutex created\n");

    //---------------------------------------------------
    xTaskCreatePinnedToCore(
        taskA,
        "TaskA",
        2048,
        NULL,
        1,
        NULL,
        0
    );

    //---------------------------------------------------
    xTaskCreatePinnedToCore(
        taskB,
        "TaskB",
        2048,
        NULL,
        1,
        NULL,
        1
    );
}



