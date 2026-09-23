#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

//---------------------------------------------------

#define TASK_PRIORITY      5
#define TASK_STACK_SIZE    4096

//---------------------------------------------------

static SemaphoreHandle_t recursiveMutex;

//---------------------------------------------------

static void innerFunction(void)
{
    printf("innerFunction: taking recursive mutex\n");

    if (xSemaphoreTakeRecursive(
            recursiveMutex,
            portMAX_DELAY) == pdTRUE)
    {
        printf("innerFunction: mutex taken again\n");

        printf("innerFunction: doing protected work\n");

        vTaskDelay(pdMS_TO_TICKS(500));

        xSemaphoreGiveRecursive(recursiveMutex);

        printf("innerFunction: recursive mutex released\n");
    }
}

//---------------------------------------------------

static void workerTask(void *pvParameters)
{
    while (1)
    {
        printf("\nWorkerTask: taking recursive mutex\n");

        if (xSemaphoreTakeRecursive(
                recursiveMutex,
                portMAX_DELAY) == pdTRUE)
        {
            printf("WorkerTask: mutex taken\n");

            printf("WorkerTask: calling innerFunction()\n");

            innerFunction();

            printf("WorkerTask: returning from innerFunction()\n");

            xSemaphoreGiveRecursive(recursiveMutex);

            printf("WorkerTask: mutex completely released\n");
        }

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

//---------------------------------------------------

void app_main(void)
{
    recursiveMutex = xSemaphoreCreateRecursiveMutex();

    if (recursiveMutex == NULL)
    {
        printf("Failed to create recursive mutex\n");
        return;
    }

    printf("Recursive mutex demonstration started\n");

    xTaskCreate(
        workerTask,
        "WorkerTask",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        NULL
    );
}

//---------------------------------------------------



