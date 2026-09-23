#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

#include "esp_heap_caps.h"
#include "esp_timer.h"

//---------------------------------------------------

#define TASK_STACK_SIZE      4096

#define SENSOR_PRIORITY      5
#define PROCESS_PRIORITY     5
#define OUTPUT_PRIORITY      5
#define LOGGER_PRIORITY      5
#define MONITOR_PRIORITY     6

#define QUEUE_LENGTH         5

//---------------------------------------------------

typedef struct
{
    int sensorId;
    int value;
} SensorData;

//---------------------------------------------------

typedef struct
{
    int sensorId;
    int processedValue;
} ProcessedData;

//---------------------------------------------------

static QueueHandle_t sensorQueue;
static QueueHandle_t processedQueue;
static QueueHandle_t logQueue;

//---------------------------------------------------

static SemaphoreHandle_t sharedDataMutex;

//---------------------------------------------------

static TaskHandle_t processorTaskHandle = NULL;
static TaskHandle_t loggerTaskHandle = NULL;

//---------------------------------------------------

static int processedCount = 0;

//---------------------------------------------------

static void sensorTask(void *pvParameters)
{
    SensorData data;

    int value = 0;

    while (1)
    {
        value++;

        data.sensorId = 1;
        data.value = value;

        printf(
            "[SENSOR][Core %d] Value = %d\n",
            xPortGetCoreID(),
            data.value
        );

        if (xQueueSend(
                sensorQueue,
                &data,
                portMAX_DELAY) == pdPASS)
        {
            printf("[SENSOR] Data sent\n");
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------

static void processorTask(void *pvParameters)
{
    SensorData sensorData;
    ProcessedData processedData;

    while (1)
    {
        if (xQueueReceive(
                sensorQueue,
                &sensorData,
                portMAX_DELAY) == pdPASS)
        {
            printf(
                "[PROCESSOR][Core %d] Received = %d\n",
                xPortGetCoreID(),
                sensorData.value
            );

            processedData.sensorId =
                sensorData.sensorId;

            processedData.processedValue =
                sensorData.value * 2;

            //---------------------------------------------------

            if (xSemaphoreTake(
                    sharedDataMutex,
                    portMAX_DELAY) == pdTRUE)
            {
                processedCount++;

                xSemaphoreGive(
                    sharedDataMutex
                );
            }

            //---------------------------------------------------

            xQueueSend(
                processedQueue,
                &processedData,
                portMAX_DELAY
            );

            //---------------------------------------------------

            xTaskNotifyGive(
                loggerTaskHandle
            );
        }
    }
}

//---------------------------------------------------

static void outputTask(void *pvParameters)
{
    ProcessedData data;

    while (1)
    {
        if (xQueueReceive(
                processedQueue,
                &data,
                portMAX_DELAY) == pdPASS)
        {
            printf(
                "[OUTPUT][Core %d] Sensor %d -> %d\n",
                xPortGetCoreID(),
                data.sensorId,
                data.processedValue
            );
        }
    }
}

//---------------------------------------------------

static void loggerTask(void *pvParameters)
{
    while (1)
    {
        ulTaskNotifyTake(
            pdTRUE,
            portMAX_DELAY
        );

        if (xSemaphoreTake(
                sharedDataMutex,
                portMAX_DELAY) == pdTRUE)
        {
            printf(
                "[LOGGER][Core %d] "
                "Processed count = %d\n",
                xPortGetCoreID(),
                processedCount
            );

            xSemaphoreGive(
                sharedDataMutex
            );
        }
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
    UBaseType_t stackFree;
    BaseType_t affinity;

    state = eTaskGetState(handle);

    priority = uxTaskPriorityGet(handle);

    stackFree =
        uxTaskGetStackHighWaterMark(handle);

    affinity =
        xTaskGetAffinity(handle);

    printf(
        "%-12s State=%-9s "
        "Priority=%lu "
        "StackFree=%lu words "
        "Affinity=%ld\n",
        name,

        state == eRunning   ? "RUNNING" :
        state == eReady     ? "READY" :
        state == eBlocked   ? "BLOCKED" :
        state == eSuspended ? "SUSPENDED" :
        state == eDeleted   ? "DELETED" :
                               "UNKNOWN",

        (unsigned long)priority,

        (unsigned long)stackFree,

        (long)affinity
    );
}

//---------------------------------------------------

static void monitorTask(void *pvParameters)
{
    while (1)
    {
        uint32_t freeHeap;
        uint32_t minimumHeap;

        freeHeap =
            esp_get_free_heap_size();

        minimumHeap =
            esp_get_minimum_free_heap_size();

        printf("\n");
        printf(
            "============================================================\n"
        );

        printf(
            "                 FREERTOS FULL SYSTEM\n"
        );

        printf(
            "============================================================\n"
        );

        printf(
            "Uptime       : %lld ms\n",
            esp_timer_get_time() / 1000
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
            "Processed    : %d\n",
            processedCount
        );

        printf(
            "------------------------------------------------------------\n"
        );

        printTaskInfo(
            "Sensor",
            xTaskGetHandle("SensorTask")
        );

        printTaskInfo(
            "Processor",
            processorTaskHandle
        );

        printTaskInfo(
            "Output",
            xTaskGetHandle("OutputTask")
        );

        printTaskInfo(
            "Logger",
            loggerTaskHandle
        );

        printf(
            "============================================================\n"
        );

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

//---------------------------------------------------

void app_main(void)
{
    //---------------------------------------------------
    // Create queues
    //---------------------------------------------------

    sensorQueue = xQueueCreate(
        QUEUE_LENGTH,
        sizeof(SensorData)
    );

    processedQueue = xQueueCreate(
        QUEUE_LENGTH,
        sizeof(ProcessedData)
    );

    logQueue = xQueueCreate(
        QUEUE_LENGTH,
        sizeof(int)
    );

    //---------------------------------------------------
    // Create mutex
    //---------------------------------------------------

    sharedDataMutex =
        xSemaphoreCreateMutex();

    //---------------------------------------------------

    if (sensorQueue == NULL ||
        processedQueue == NULL ||
        logQueue == NULL ||
        sharedDataMutex == NULL)
    {
        printf(
            "Failed to create FreeRTOS objects\n"
        );

        return;
    }

    //---------------------------------------------------
    // Sensor Task
    //---------------------------------------------------

    xTaskCreatePinnedToCore(
        sensorTask,
        "SensorTask",
        TASK_STACK_SIZE,
        NULL,
        SENSOR_PRIORITY,
        NULL,
        0
    );

    //---------------------------------------------------
    // Processor Task
    //---------------------------------------------------

    xTaskCreatePinnedToCore(
        processorTask,
        "ProcessorTask",
        TASK_STACK_SIZE,
        NULL,
        PROCESS_PRIORITY,
        &processorTaskHandle,
        1
    );

    //---------------------------------------------------
    // Output Task
    //---------------------------------------------------

    xTaskCreatePinnedToCore(
        outputTask,
        "OutputTask",
        TASK_STACK_SIZE,
        NULL,
        OUTPUT_PRIORITY,
        NULL,
        0
    );

    //---------------------------------------------------
    // Logger Task
    //---------------------------------------------------

    xTaskCreatePinnedToCore(
        loggerTask,
        "LoggerTask",
        TASK_STACK_SIZE,
        NULL,
        LOGGER_PRIORITY,
        &loggerTaskHandle,
        1
    );

    //---------------------------------------------------
    // Monitor Task
    //---------------------------------------------------

    xTaskCreate(
        monitorTask,
        "MonitorTask",
        TASK_STACK_SIZE,
        NULL,
        MONITOR_PRIORITY,
        NULL
    );

    //---------------------------------------------------

    printf(
        "\nFreeRTOS full system started\n"
    );

    printf(
        "Sensor -> Processor -> Output\n"
    );

    printf(
        "Processor -> Logger notification\n"
    );

    printf(
        "Monitor -> System diagnostics\n"
    );

    //---------------------------------------------------
}



