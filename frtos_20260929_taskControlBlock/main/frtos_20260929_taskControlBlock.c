#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//---------------------------------------------------

#define TASK_STACK_SIZE    4096
#define TASK_PRIORITY      5

//---------------------------------------------------

static TaskHandle_t workerTaskHandle;

//---------------------------------------------------

static void workerTask(void *pvParameters)
{
    while (1)
    {
        printf(
            "[WORKER] Running on Core %d\n",
            xPortGetCoreID()
        );

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------

static void monitorTask(void *pvParameters)
{
    while (1)
    {
        printf("\n");
        printf("========================================\n");
        printf("          TASK CONTROL BLOCK\n");
        printf("========================================\n");

        printf(
            "Task Handle       : %p\n",
            (void *)workerTaskHandle
        );

        printf(
            "Task State        : %d\n",
            eTaskGetState(workerTaskHandle)
        );

        printf(
            "Task Priority     : %lu\n",
            (unsigned long)uxTaskPriorityGet(
                workerTaskHandle
            )
        );

        printf(
            "Stack Free        : %u words\n",
            (unsigned int)uxTaskGetStackHighWaterMark(
                workerTaskHandle
            )
        );

        printf(
            "Current Core      : %d\n",
            xPortGetCoreID()
        );

        printf("========================================\n\n");

        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}

//---------------------------------------------------

void app_main(void)
{
    xTaskCreatePinnedToCore(
        workerTask,
        "WorkerTask",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        &workerTaskHandle,
        0
    );

    xTaskCreatePinnedToCore(
        monitorTask,
        "MonitorTask",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        NULL,
        1
    );

    printf("Task Control Block demonstration started\n");

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------



