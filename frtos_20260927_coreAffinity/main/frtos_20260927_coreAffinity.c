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
        printf("Task 1: Core affinity = Core 0, running on CPU %d\n",
               xPortGetCoreID());

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------

static void core1Task(void *pvParameters)
{
    while (1)
    {
        printf("Task 2: Core affinity = Core 1, running on CPU %d\n",
               xPortGetCoreID());

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------

static void unpinnedTask(void *pvParameters)
{
    while (1)
    {
        printf("Task 3: Unpinned, running on CPU %d\n",
               xPortGetCoreID());

        vTaskDelay(pdMS_TO_TICKS(1000));
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

    xTaskCreate(
        unpinnedTask,
        "UnpinnedTask",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        NULL
    );

    printf("Core affinity demonstration started\n");

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------



