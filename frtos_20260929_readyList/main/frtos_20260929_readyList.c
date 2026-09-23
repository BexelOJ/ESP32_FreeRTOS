#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//---------------------------------------------------

#define TASK_STACK_SIZE    4096

#define LOW_PRIORITY       3
#define MEDIUM_PRIORITY    5
#define HIGH_PRIORITY      7

//---------------------------------------------------

static void lowPriorityTask(void *pvParameters)
{
    while (1)
    {
        printf(
            "[LOW]    Priority = %d, Core = %d\n",
            LOW_PRIORITY,
            xPortGetCoreID()
        );

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

//---------------------------------------------------

static void mediumPriorityTask(void *pvParameters)
{
    while (1)
    {
        printf(
            "[MEDIUM] Priority = %d, Core = %d\n",
            MEDIUM_PRIORITY,
            xPortGetCoreID()
        );

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

//---------------------------------------------------

static void highPriorityTask(void *pvParameters)
{
    while (1)
    {
        printf(
            "[HIGH]   Priority = %d, Core = %d\n",
            HIGH_PRIORITY,
            xPortGetCoreID()
        );

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

//---------------------------------------------------

void app_main(void)
{
    xTaskCreatePinnedToCore(
        lowPriorityTask,
        "LowPriorityTask",
        TASK_STACK_SIZE,
        NULL,
        LOW_PRIORITY,
        NULL,
        0
    );

    xTaskCreatePinnedToCore(
        mediumPriorityTask,
        "MediumPriorityTask",
        TASK_STACK_SIZE,
        NULL,
        MEDIUM_PRIORITY,
        NULL,
        0
    );

    xTaskCreatePinnedToCore(
        highPriorityTask,
        "HighPriorityTask",
        TASK_STACK_SIZE,
        NULL,
        HIGH_PRIORITY,
        NULL,
        0
    );

    printf("Ready list demonstration started\n");

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------



