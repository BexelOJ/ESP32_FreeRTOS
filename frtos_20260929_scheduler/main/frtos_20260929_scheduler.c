#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//---------------------------------------------------

#define TASK_STACK_SIZE    4096

#define LOW_PRIORITY       3
#define MEDIUM_PRIORITY    5
#define HIGH_PRIORITY      7

//---------------------------------------------------

static void lowTask(void *pvParameters)
{
    while (1)
    {
        printf(
            "[LOW]    Running | Priority = %d | Core = %d\n",
            LOW_PRIORITY,
            xPortGetCoreID()
        );

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------

static void mediumTask(void *pvParameters)
{
    while (1)
    {
        printf(
            "[MEDIUM] Running | Priority = %d | Core = %d\n",
            MEDIUM_PRIORITY,
            xPortGetCoreID()
        );

        vTaskDelay(pdMS_TO_TICKS(1500));
    }
}

//---------------------------------------------------

static void highTask(void *pvParameters)
{
    while (1)
    {
        printf(
            "[HIGH]   Running | Priority = %d | Core = %d\n",
            HIGH_PRIORITY,
            xPortGetCoreID()
        );

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

//---------------------------------------------------

void app_main(void)
{
    xTaskCreatePinnedToCore(
        lowTask,
        "LowTask",
        TASK_STACK_SIZE,
        NULL,
        LOW_PRIORITY,
        NULL,
        0
    );

    xTaskCreatePinnedToCore(
        mediumTask,
        "MediumTask",
        TASK_STACK_SIZE,
        NULL,
        MEDIUM_PRIORITY,
        NULL,
        0
    );

    xTaskCreatePinnedToCore(
        highTask,
        "HighTask",
        TASK_STACK_SIZE,
        NULL,
        HIGH_PRIORITY,
        NULL,
        0
    );

    printf("Scheduler demonstration started\n");

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------



