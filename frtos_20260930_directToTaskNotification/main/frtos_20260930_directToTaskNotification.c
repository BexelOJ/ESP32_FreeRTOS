#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//---------------------------------------------------

#define TASK_PRIORITY      5
#define TASK_STACK_SIZE    4096

//---------------------------------------------------

static TaskHandle_t workerTaskHandle = NULL;

//---------------------------------------------------

static void producerTask(void *pvParameters)
{
    int notificationCount = 0;

    while (1)
    {
        notificationCount++;

        printf(
            "Producer: sending notification %d\n",
            notificationCount
        );

        xTaskNotifyGive(workerTaskHandle);

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

//---------------------------------------------------

static void workerTask(void *pvParameters)
{
    while (1)
    {
        printf("Worker: waiting for notification\n");

        ulTaskNotifyTake(
            pdTRUE,
            portMAX_DELAY
        );

        printf(
            "Worker: notification received on Core %d\n",
            xPortGetCoreID()
        );

        printf("Worker: processing event\n");
    }
}

//---------------------------------------------------

void app_main(void)
{
    xTaskCreate(
        workerTask,
        "WorkerTask",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        &workerTaskHandle
    );

    xTaskCreate(
        producerTask,
        "ProducerTask",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        NULL
    );

    printf("Direct-to-task notification demonstration started\n");
}

//---------------------------------------------------



