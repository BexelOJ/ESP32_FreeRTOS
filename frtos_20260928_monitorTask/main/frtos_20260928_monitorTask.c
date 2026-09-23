#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//---------------------------------------------------

#define TASK_STACK_SIZE        4096

#define WORKER_PRIORITY        5
#define MONITOR_PRIORITY       6

//---------------------------------------------------

static TaskHandle_t workerTaskHandle;
static TaskHandle_t loggerTaskHandle;

//---------------------------------------------------

static void workerTask(void *pvParameters)
{
    int counter = 0;

    while (1)
    {
        counter++;

        printf(
            "[WORKER][Core %d] Counter = %d\n",
            xPortGetCoreID(),
            counter
        );

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------

static void loggerTask(void *pvParameters)
{
    int counter = 0;

    while (1)
    {
        counter++;

        printf(
            "[LOGGER][Core %d] Log message = %d\n",
            xPortGetCoreID(),
            counter
        );

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

//---------------------------------------------------

static void monitorTask(void *pvParameters)
{
    while (1)
    {
        printf("\n");
        printf("========================================\n");
        printf("          SYSTEM MONITOR\n");
        printf("========================================\n");

        printf(
            "Worker Task:\n"
            "  State       = %d\n"
            "  Priority    = %lu\n"
            "  Stack Free  = %u words\n",
            eTaskGetState(workerTaskHandle),
            (unsigned long)uxTaskPriorityGet(workerTaskHandle),
            (unsigned int)uxTaskGetStackHighWaterMark(
                workerTaskHandle
            )
        );

        printf(
            "Logger Task:\n"
            "  State       = %d\n"
            "  Priority    = %lu\n"
            "  Stack Free  = %u words\n",
            eTaskGetState(loggerTaskHandle),
            (unsigned long)uxTaskPriorityGet(loggerTaskHandle),
            (unsigned int)uxTaskGetStackHighWaterMark(
                loggerTaskHandle
            )
        );

        printf(
            "Monitor Task:\n"
            "  Core        = %d\n"
            "  Free Heap   = %lu bytes\n",
            xPortGetCoreID(),
            (unsigned long)esp_get_free_heap_size()
        );

        printf("========================================\n\n");

        vTaskDelay(pdMS_TO_TICKS(5000));
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
        WORKER_PRIORITY,
        &workerTaskHandle,
        0
    );

    xTaskCreatePinnedToCore(
        loggerTask,
        "LoggerTask",
        TASK_STACK_SIZE,
        NULL,
        WORKER_PRIORITY,
        &loggerTaskHandle,
        0
    );

    xTaskCreatePinnedToCore(
        monitorTask,
        "MonitorTask",
        TASK_STACK_SIZE,
        NULL,
        MONITOR_PRIORITY,
        NULL,
        1
    );

    printf("Monitor task demonstration started\n");

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------



