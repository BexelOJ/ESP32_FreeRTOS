#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//---------------------------------------------------
#define WORKER_STACK_SIZE 2048

//---------------------------------------------------
static TaskHandle_t workerTaskHandle = NULL;

//---------------------------------------------------
static void printStackUsage(
    const char *taskName,
    TaskHandle_t taskHandle)
{
    UBaseType_t highWaterMark =
        uxTaskGetStackHighWaterMark(taskHandle);

    printf("\n");
    printf("---------------------------------------------------\n");
    printf("Task              : %s\n", taskName);
    printf("Stack Size        : %d words\n", WORKER_STACK_SIZE);
    printf("Minimum Free Stack: %u words\n",
           (unsigned int)highWaterMark);
    printf("Used Stack        : %u words\n",
           (unsigned int)(WORKER_STACK_SIZE - highWaterMark));
    printf("---------------------------------------------------\n");
}

//---------------------------------------------------
static void workerTask(void *pvParameters)
{
    int counter = 0;

    while (1)
    {
        printf("Worker Task: Counter = %d\n", counter++);

        //---------------------------------------------------
        printStackUsage(
            "WorkerTask",
            workerTaskHandle
        );

        //---------------------------------------------------
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------
static void monitorTask(void *pvParameters)
{
    while (1)
    {
        if (workerTaskHandle != NULL)
        {
            UBaseType_t highWaterMark =
                uxTaskGetStackHighWaterMark(workerTaskHandle);

            printf(
                "Monitor: Worker minimum free stack = %u words\n",
                (unsigned int)highWaterMark
            );
        }

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

//---------------------------------------------------
void app_main(void)
{
    //---------------------------------------------------
    xTaskCreate(
        workerTask,
        "WorkerTask",
        WORKER_STACK_SIZE,
        NULL,
        1,
        &workerTaskHandle
    );

    //---------------------------------------------------
    xTaskCreate(
        monitorTask,
        "MonitorTask",
        2048,
        NULL,
        2,
        NULL
    );
}



