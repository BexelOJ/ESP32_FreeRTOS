#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_heap_caps.h"
#include "esp_timer.h"

//---------------------------------------------------

#define TASK_PRIORITY      5
#define MONITOR_PRIORITY   6
#define TASK_STACK_SIZE    4096

//---------------------------------------------------

static TaskHandle_t workerATaskHandle = NULL;
static TaskHandle_t workerBTaskHandle = NULL;
static TaskHandle_t workerCTaskHandle = NULL;

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
        printf("Worker A running on Core %d\n",
               xPortGetCoreID());

        for (volatile int i = 0; i < 500000; i++)
        {
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------

static void workerBTask(void *pvParameters)
{
    while (1)
    {
        printf("Worker B running on Core %d\n",
               xPortGetCoreID());

        for (volatile int i = 0; i < 800000; i++)
        {
        }

        vTaskDelay(pdMS_TO_TICKS(1500));
    }
}

//---------------------------------------------------

static void workerCTask(void *pvParameters)
{
    while (1)
    {
        printf("Worker C running on Core %d\n",
               xPortGetCoreID());

        for (volatile int i = 0; i < 1200000; i++)
        {
        }

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

//---------------------------------------------------

static void printTaskInfo(
    const char *name,
    TaskHandle_t handle
)
{
    eTaskState state;
    UBaseType_t priority;
    UBaseType_t stackHighWaterMark;
    BaseType_t core;

    state = eTaskGetState(handle);

    priority = uxTaskPriorityGet(handle);

    stackHighWaterMark =
        uxTaskGetStackHighWaterMark(handle);

    core = xTaskGetAffinity(handle);

    printf(
        "%-12s %-10s Priority=%lu "
        "StackFree=%lu words "
        "Affinity=%ld\n",
        name,
        taskStateToString(state),
        (unsigned long)priority,
        (unsigned long)stackHighWaterMark,
        (long)core
    );
}

//---------------------------------------------------

static void systemMonitorTask(void *pvParameters)
{
    while (1)
    {
        uint32_t freeHeap;
        uint32_t minimumHeap;
        uint32_t taskCount;
        int64_t uptime;

        freeHeap = esp_get_free_heap_size();

        minimumHeap =
            esp_get_minimum_free_heap_size();

        taskCount = uxTaskGetNumberOfTasks();

        uptime = esp_timer_get_time();

        printf("\n");
        printf("============================================================\n");
        printf("                 FreeRTOS SYSTEM MONITOR\n");
        printf("============================================================\n");

        printf(
            "Uptime       : %lld ms\n",
            uptime / 1000
        );

        printf(
            "Free Heap    : %lu bytes\n",
            (unsigned long)freeHeap
        );

        printf(
            "Minimum Heap : %lu bytes\n",
            (unsigned long)minimumHeap
        );

        printf(
            "Task Count   : %lu\n",
            (unsigned long)taskCount
        );

        printf("------------------------------------------------------------\n");

        printTaskInfo(
            "Worker A",
            workerATaskHandle
        );

        printTaskInfo(
            "Worker B",
            workerBTaskHandle
        );

        printTaskInfo(
            "Worker C",
            workerCTaskHandle
        );

        printf("============================================================\n");

        vTaskDelay(pdMS_TO_TICKS(3000));
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
        workerCTask,
        "WorkerC",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        &workerCTaskHandle
    );

    xTaskCreate(
        systemMonitorTask,
        "SystemMonitor",
        TASK_STACK_SIZE,
        NULL,
        MONITOR_PRIORITY,
        NULL
    );

    printf("FreeRTOS system monitor started\n");
}

//---------------------------------------------------



