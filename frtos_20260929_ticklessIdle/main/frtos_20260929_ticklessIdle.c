#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//---------------------------------------------------

#define TASK_STACK_SIZE    4096
#define TASK_PRIORITY      5

//---------------------------------------------------

static void periodicTask(void *pvParameters)
{
    while (1)
    {
        TickType_t currentTick = xTaskGetTickCount();

        printf(
            "[TASK] Running | Tick = %lu | Core = %d\n",
            (unsigned long)currentTick,
            xPortGetCoreID()
        );

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

//---------------------------------------------------

void app_main(void)
{
    printf("Tickless idle demonstration started\n");

    xTaskCreatePinnedToCore(
        periodicTask,
        "PeriodicTask",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        NULL,
        0
    );

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}

//---------------------------------------------------



