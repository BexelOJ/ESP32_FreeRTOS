#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

//---------------------------------------------------

#define PRODUCER_PRIORITY      4
#define COMMAND_PRIORITY       5

#define TASK_STACK_SIZE        4096
#define COMMAND_QUEUE_LENGTH   5
#define COMMAND_SIZE           32

//---------------------------------------------------

typedef struct
{
    char command[COMMAND_SIZE];

} Command;

//---------------------------------------------------

static QueueHandle_t commandQueue;

//---------------------------------------------------

static void commandProducerTask(void *pvParameters)
{
    const char *commands[] =
    {
        "LED_ON",
        "STATUS",
        "LED_OFF",
        "STATUS"
    };

    int commandIndex = 0;

    while (1)
    {
        Command command;

        snprintf(
            command.command,
            COMMAND_SIZE,
            "%s",
            commands[commandIndex]
        );

        printf(
            "Producer: sending command = %s\n",
            command.command
        );

        if (xQueueSend(
                commandQueue,
                &command,
                pdMS_TO_TICKS(100)) != pdPASS)
        {
            printf("Command queue full\n");
        }

        commandIndex++;

        if (commandIndex >= 4)
        {
            commandIndex = 0;
        }

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

//---------------------------------------------------

static void commandTask(void *pvParameters)
{
    Command command;

    while (1)
    {
        if (xQueueReceive(
                commandQueue,
                &command,
                portMAX_DELAY) == pdPASS)
        {
            printf(
                "Command Task: Core %d | "
                "received = %s\n",
                xPortGetCoreID(),
                command.command
            );

            if (strcmp(command.command, "LED_ON") == 0)
            {
                printf("Command Task: Turning LED ON\n");
            }
            else if (strcmp(command.command, "LED_OFF") == 0)
            {
                printf("Command Task: Turning LED OFF\n");
            }
            else if (strcmp(command.command, "STATUS") == 0)
            {
                printf("Command Task: System STATUS requested\n");
            }
            else
            {
                printf(
                    "Command Task: Unknown command = %s\n",
                    command.command
                );
            }

            printf("\n");
        }
    }
}

//---------------------------------------------------

void app_main(void)
{
    commandQueue = xQueueCreate(
        COMMAND_QUEUE_LENGTH,
        sizeof(Command)
    );

    if (commandQueue == NULL)
    {
        printf("Failed to create command queue\n");
        return;
    }

    xTaskCreatePinnedToCore(
        commandProducerTask,
        "CommandProducer",
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
        COMMAND_PRIORITY,
        NULL,
        1
    );

    printf("Command task demonstration started\n");

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------



