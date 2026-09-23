#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

//---------------------------------------------------

#define TASK_STACK_SIZE        4096

#define DEVICE_MANAGER_PRIORITY    6
#define DEVICE_TASK_PRIORITY       5

#define DEVICE_QUEUE_LENGTH     10

//---------------------------------------------------

typedef enum
{
    DEVICE_LED,
    DEVICE_MOTOR,
    DEVICE_SENSOR

} DeviceType;

//---------------------------------------------------

typedef enum
{
    DEVICE_COMMAND_ON,
    DEVICE_COMMAND_OFF,
    DEVICE_COMMAND_STATUS

} DeviceCommand;

//---------------------------------------------------

typedef struct
{
    DeviceType device;
    DeviceCommand command;

} DeviceRequest;

//---------------------------------------------------

static QueueHandle_t deviceQueue;

//---------------------------------------------------

static const char *deviceName(DeviceType device)
{
    switch (device)
    {
        case DEVICE_LED:
            return "LED";

        case DEVICE_MOTOR:
            return "MOTOR";

        case DEVICE_SENSOR:
            return "SENSOR";

        default:
            return "UNKNOWN";
    }
}

//---------------------------------------------------

static const char *commandName(DeviceCommand command)
{
    switch (command)
    {
        case DEVICE_COMMAND_ON:
            return "ON";

        case DEVICE_COMMAND_OFF:
            return "OFF";

        case DEVICE_COMMAND_STATUS:
            return "STATUS";

        default:
            return "UNKNOWN";
    }
}

//---------------------------------------------------

static void handleDeviceRequest(DeviceRequest *request)
{
    printf(
        "[DEVICE MANAGER][Core %d] Device = %s, Command = %s\n",
        xPortGetCoreID(),
        deviceName(request->device),
        commandName(request->command)
    );

    switch (request->device)
    {
        case DEVICE_LED:

            if (request->command == DEVICE_COMMAND_ON)
            {
                printf("[LED] Turning ON\n");
            }
            else if (request->command == DEVICE_COMMAND_OFF)
            {
                printf("[LED] Turning OFF\n");
            }
            else
            {
                printf("[LED] Status requested\n");
            }

            break;

        case DEVICE_MOTOR:

            if (request->command == DEVICE_COMMAND_ON)
            {
                printf("[MOTOR] Starting\n");
            }
            else if (request->command == DEVICE_COMMAND_OFF)
            {
                printf("[MOTOR] Stopping\n");
            }
            else
            {
                printf("[MOTOR] Status requested\n");
            }

            break;

        case DEVICE_SENSOR:

            if (request->command == DEVICE_COMMAND_STATUS)
            {
                printf("[SENSOR] Reading sensor status\n");
            }
            else
            {
                printf("[SENSOR] Command not supported\n");
            }

            break;

        default:
            printf("[DEVICE MANAGER] Unknown device\n");
            break;
    }
}

//---------------------------------------------------

static void deviceManagerTask(void *pvParameters)
{
    DeviceRequest request;

    while (1)
    {
        if (xQueueReceive(
                deviceQueue,
                &request,
                portMAX_DELAY) == pdPASS)
        {
            handleDeviceRequest(&request);
        }
    }
}

//---------------------------------------------------

static void commandProducerTask(void *pvParameters)
{
    DeviceRequest request;

    while (1)
    {
        request.device = DEVICE_LED;
        request.command = DEVICE_COMMAND_ON;

        xQueueSend(
            deviceQueue,
            &request,
            pdMS_TO_TICKS(100)
        );

        vTaskDelay(pdMS_TO_TICKS(1000));

        request.device = DEVICE_MOTOR;
        request.command = DEVICE_COMMAND_ON;

        xQueueSend(
            deviceQueue,
            &request,
            pdMS_TO_TICKS(100)
        );

        vTaskDelay(pdMS_TO_TICKS(1000));

        request.device = DEVICE_SENSOR;
        request.command = DEVICE_COMMAND_STATUS;

        xQueueSend(
            deviceQueue,
            &request,
            pdMS_TO_TICKS(100)
        );

        vTaskDelay(pdMS_TO_TICKS(1000));

        request.device = DEVICE_MOTOR;
        request.command = DEVICE_COMMAND_OFF;

        xQueueSend(
            deviceQueue,
            &request,
            pdMS_TO_TICKS(100)
        );

        vTaskDelay(pdMS_TO_TICKS(1000));

        request.device = DEVICE_LED;
        request.command = DEVICE_COMMAND_OFF;

        xQueueSend(
            deviceQueue,
            &request,
            pdMS_TO_TICKS(100)
        );

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

//---------------------------------------------------

void app_main(void)
{
    deviceQueue = xQueueCreate(
        DEVICE_QUEUE_LENGTH,
        sizeof(DeviceRequest)
    );

    if (deviceQueue == NULL)
    {
        printf("Failed to create device queue\n");
        return;
    }

    xTaskCreatePinnedToCore(
        deviceManagerTask,
        "DeviceManagerTask",
        TASK_STACK_SIZE,
        NULL,
        DEVICE_MANAGER_PRIORITY,
        NULL,
        1
    );

    xTaskCreatePinnedToCore(
        commandProducerTask,
        "CommandProducerTask",
        TASK_STACK_SIZE,
        NULL,
        DEVICE_TASK_PRIORITY,
        NULL,
        0
    );

    printf("Device manager demonstration started\n");

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------



