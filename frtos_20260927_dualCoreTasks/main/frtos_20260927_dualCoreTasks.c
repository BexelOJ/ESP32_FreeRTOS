#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//---------------------------------------------------

#define TASK_PRIORITY    5
#define TASK_STACK_SIZE  4096

//---------------------------------------------------

static void core0Task(void *pvParameters)
{
    while (1)
    {
        printf("Core 0 Task running on CPU %d\n", xPortGetCoreID());

        for (volatile int i = 0; i < 1000000; i++)
        {
        }

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

//---------------------------------------------------

static void core1Task(void *pvParameters)
{
    while (1)
    {
        printf("Core 1 Task running on CPU %d\n", xPortGetCoreID());

        for (volatile int i = 0; i < 1000000; i++)
        {
        }

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

//---------------------------------------------------

void app_main(void)
{
    xTaskCreatePinnedToCore(
        core0Task,
        "Core0Task",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        NULL,
        0
    );

    xTaskCreatePinnedToCore(
        core1Task,
        "Core1Task",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        NULL,
        1
    );

    printf("Dual-core task demonstration started\n");

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------



