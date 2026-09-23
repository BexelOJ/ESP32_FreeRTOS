#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

//---------------------------------------------------

#define TASK_STACK_SIZE        4096

#define SENSOR_PRIORITY        5
#define PROCESS_PRIORITY       5
#define OUTPUT_PRIORITY        5

#define QUEUE_LENGTH           5

//---------------------------------------------------

typedef struct
{
    int sensorValue;

} SensorData;

//---------------------------------------------------

typedef struct
{
    int processedValue;

} ProcessedData;

//---------------------------------------------------

static QueueHandle_t sensorQueue;
static QueueHandle_t processedQueue;

//---------------------------------------------------

static void sensorTask(void *pvParameters)
{
    int value = 0;

    while (1)
    {
        SensorData data;

        value += 10;

        data.sensorValue = value;

        printf(
            "[SENSOR][Core %d] Generated value = %d\n",
            xPortGetCoreID(),
            data.sensorValue
        );

        if (xQueueSend(
                sensorQueue,
                &data,
                pdMS_TO_TICKS(100)) != pdPASS)
        {
            printf("[SENSOR] Queue full\n");
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------

static void processingTask(void *pvParameters)
{
    SensorData sensorData;

    while (1)
    {
        if (xQueueReceive(
                sensorQueue,
                &sensorData,
                portMAX_DELAY) == pdPASS)
        {
            ProcessedData processedData;

            processedData.processedValue =
                sensorData.sensorValue * 2;

            printf(
                "[PROCESS][Core %d] %d -> %d\n",
                xPortGetCoreID(),
                sensorData.sensorValue,
                processedData.processedValue
            );

            if (xQueueSend(
                    processedQueue,
                    &processedData,
                    pdMS_TO_TICKS(100)) != pdPASS)
            {
                printf("[PROCESS] Queue full\n");
            }
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
                "[OUTPUT][Core %d] Final value = %d\n",
                xPortGetCoreID(),
                data.processedValue
            );
        }
    }
}

//---------------------------------------------------

void app_main(void)
{
    sensorQueue = xQueueCreate(
        QUEUE_LENGTH,
        sizeof(SensorData)
    );

    processedQueue = xQueueCreate(
        QUEUE_LENGTH,
        sizeof(ProcessedData)
    );

    if (sensorQueue == NULL || processedQueue == NULL)
    {
        printf("Failed to create queues\n");
        return;
    }

    xTaskCreatePinnedToCore(
        sensorTask,
        "SensorTask",
        TASK_STACK_SIZE,
        NULL,
        SENSOR_PRIORITY,
        NULL,
        0
    );

    xTaskCreatePinnedToCore(
        processingTask,
        "ProcessingTask",
        TASK_STACK_SIZE,
        NULL,
        PROCESS_PRIORITY,
        NULL,
        1
    );

    xTaskCreatePinnedToCore(
        outputTask,
        "OutputTask",
        TASK_STACK_SIZE,
        NULL,
        OUTPUT_PRIORITY,
        NULL,
        0
    );

    printf("Data pipeline demonstration started\n");

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------



