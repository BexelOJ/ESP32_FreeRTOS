#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

//---------------------------------------------------

#define LOGGER_PRIORITY        6
#define PRODUCER_PRIORITY      5

#define TASK_STACK_SIZE        4096

#define LOG_QUEUE_LENGTH       10
#define LOG_MESSAGE_SIZE       64

//---------------------------------------------------

typedef struct
{
    char message[LOG_MESSAGE_SIZE];

} LogMessage;

//---------------------------------------------------

static QueueHandle_t logQueue;

//---------------------------------------------------

static void loggerTask(void *pvParameters)
{
    LogMessage logMessage;

    while (1)
    {
        if (xQueueReceive(
                logQueue,
                &logMessage,
                portMAX_DELAY) == pdPASS)
        {
            printf(
                "[LOGGER][Core %d] %s\n",
                xPortGetCoreID(),
                logMessage.message
            );
        }
    }
}

//---------------------------------------------------

static void sensorTask(void *pvParameters)
{
    int sensorValue = 0;

    while (1)
    {
        sensorValue += 10;

        LogMessage logMessage;

        snprintf(
            logMessage.message,
            LOG_MESSAGE_SIZE,
            "Sensor value = %d",
            sensorValue
        );

        xQueueSend(
            logQueue,
            &logMessage,
            pdMS_TO_TICKS(100)
        );

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------

static void commandTask(void *pvParameters)
{
    int commandNumber = 0;

    while (1)
    {
        commandNumber++;

        LogMessage logMessage;

        snprintf(
            logMessage.message,
            LOG_MESSAGE_SIZE,
            "Command processed = %d",
            commandNumber
        );

        xQueueSend(
            logQueue,
            &logMessage,
            pdMS_TO_TICKS(100)
        );

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

//---------------------------------------------------

void app_main(void)
{
    logQueue = xQueueCreate(
        LOG_QUEUE_LENGTH,
        sizeof(LogMessage)
    );

    if (logQueue == NULL)
    {
        printf("Failed to create log queue\n");
        return;
    }

    xTaskCreatePinnedToCore(
        loggerTask,
        "LoggerTask",
        TASK_STACK_SIZE,
        NULL,
        LOGGER_PRIORITY,
        NULL,
        1
    );

    xTaskCreatePinnedToCore(
        sensorTask,
        "SensorTask",
        TASK_STACK_SIZE,
        NULL,
        PRODUCER_PRIORITY,
        NULL,
        0
    );

    xTaskCreatePinnedToCore(
        commandTask,
        "CommandTask",
        TASK_STACK_SIZE,
        NULL,
        PRODUCER_PRIORITY,
        NULL,
        0
    );

    printf("Logger task demonstration started\n");

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------



