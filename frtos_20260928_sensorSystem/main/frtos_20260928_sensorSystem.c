#include <stdio.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

//---------------------------------------------------

#define SENSOR_TASK_PRIORITY       5
#define PROCESSOR_TASK_PRIORITY    5

#define TASK_STACK_SIZE            4096
#define SENSOR_QUEUE_LENGTH        10

//---------------------------------------------------

typedef struct
{
    float temperature;
    float humidity;
    int sensorId;

} SensorData;

//---------------------------------------------------

static QueueHandle_t sensorQueue;

//---------------------------------------------------

static void sensorTask(void *pvParameters)
{
    int sensorId = 1;

    while (1)
    {
        SensorData data;

        data.sensorId = sensorId;

        data.temperature =
            20.0f + (float)(rand() % 150) / 10.0f;

        data.humidity =
            40.0f + (float)(rand() % 300) / 10.0f;

        printf(
            "Sensor Task: Core %d | "
            "Sensor = %d | "
            "Temperature = %.1f C | "
            "Humidity = %.1f %%\n",
            xPortGetCoreID(),
            data.sensorId,
            data.temperature,
            data.humidity
        );

        if (xQueueSend(
                sensorQueue,
                &data,
                pdMS_TO_TICKS(100)) != pdPASS)
        {
            printf("Sensor Queue full\n");
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------

static void processorTask(void *pvParameters)
{
    SensorData data;

    while (1)
    {
        if (xQueueReceive(
                sensorQueue,
                &data,
                portMAX_DELAY) == pdPASS)
        {
            printf(
                "Processor Task: Core %d | "
                "Sensor = %d | "
                "Temperature = %.1f C | "
                "Humidity = %.1f %%\n",
                xPortGetCoreID(),
                data.sensorId,
                data.temperature,
                data.humidity
            );

            if (data.temperature > 30.0f)
            {
                printf("WARNING: High temperature\n");
            }

            if (data.humidity > 70.0f)
            {
                printf("WARNING: High humidity\n");
            }

            printf("\n");
        }
    }
}

//---------------------------------------------------

void app_main(void)
{
    sensorQueue = xQueueCreate(
        SENSOR_QUEUE_LENGTH,
        sizeof(SensorData)
    );

    if (sensorQueue == NULL)
    {
        printf("Failed to create sensor queue\n");
        return;
    }

    xTaskCreatePinnedToCore(
        sensorTask,
        "SensorTask",
        TASK_STACK_SIZE,
        NULL,
        SENSOR_TASK_PRIORITY,
        NULL,
        0
    );

    xTaskCreatePinnedToCore(
        processorTask,
        "ProcessorTask",
        TASK_STACK_SIZE,
        NULL,
        PROCESSOR_TASK_PRIORITY,
        NULL,
        1
    );

    printf("Sensor system started\n");

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------



