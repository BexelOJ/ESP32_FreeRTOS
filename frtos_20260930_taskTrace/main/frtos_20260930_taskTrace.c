#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//---------------------------------------------------

#define TASK_PRIORITY      5
#define TASK_STACK_SIZE    4096

//---------------------------------------------------

static TaskHandle_t workerATaskHandle = NULL;
static TaskHandle_t workerBTaskHandle = NULL;

//---------------------------------------------------

static const char *taskStateToString(
    eTaskState state
)
{
    switch (state)
    {
        case eRunning:
            return "RUNNING";

        case eReady:
            return "READY";

        case eBlocked:
            return "BLOCKED";

        case eSuspended:
            return "SUSPENDED";

        case eDeleted:
            return "DELETED";

        default:
            return "UNKNOWN";
    }
}

//---------------------------------------------------

static void workerATask(void *pvParameters)
{
    while (1)
    {
        printf(
            "[TRACE] Worker A running on Core %d\n",
            xPortGetCoreID()
        );

        for (volatile int i = 0; i < 500000; i++)
        {
        }

        printf("[TRACE] Worker A blocking\n");

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------

static void workerBTask(void *pvParameters)
{
    while (1)
    {
        printf(
            "[TRACE] Worker B running on Core %d\n",
            xPortGetCoreID()
        );

        for (volatile int i = 0; i < 800000; i++)
        {
        }

        printf("[TRACE] Worker B blocking\n");

        vTaskDelay(pdMS_TO_TICKS(1500));
    }
}

//---------------------------------------------------

static void traceTask(void *pvParameters)
{
    while (1)
    {
        eTaskState stateA;
        eTaskState stateB;

        stateA = eTaskGetState(workerATaskHandle);
        stateB = eTaskGetState(workerBTaskHandle);

        printf("\n");
        printf("============================================\n");
        printf("FreeRTOS Task Trace\n");
        printf("============================================\n");

        printf(
            "Worker A : %-9s | Priority: %lu | Core: %d\n",
            taskStateToString(stateA),
            (unsigned long)uxTaskPriorityGet(workerATaskHandle),
            xTaskGetAffinity(workerATaskHandle)
        );

        printf(
            "Worker B : %-9s | Priority: %lu | Core: %d\n",
            taskStateToString(stateB),
            (unsigned long)uxTaskPriorityGet(workerBTaskHandle),
            xTaskGetAffinity(workerBTaskHandle)
        );

        printf("============================================\n");

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

//---------------------------------------------------

void app_main(void)
{
    xTaskCreatePinnedToCore(
        workerATask,
        "WorkerA",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        &workerATaskHandle,
        0
    );

    xTaskCreatePinnedToCore(
        workerBTask,
        "WorkerB",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        &workerBTaskHandle,
        1
    );

    xTaskCreate(
        traceTask,
        "TraceTask",
        TASK_STACK_SIZE,
        NULL,
        6,
        NULL
    );

    printf("Task trace demonstration started\n");
}

//---------------------------------------------------



