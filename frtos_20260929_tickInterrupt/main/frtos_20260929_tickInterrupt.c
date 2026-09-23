#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//---------------------------------------------------

#define TASK_STACK_SIZE    4096
#define TASK_PRIORITY      5

//---------------------------------------------------

static void tickTask(void *pvParameters)
{
    TickType_t previousTick = xTaskGetTickCount();

    while (1)
    {
        TickType_t currentTick = xTaskGetTickCount();

        printf(
            "[TASK] Tick = %lu | Elapsed = %lu ticks\n",
            (unsigned long)currentTick,
            (unsigned long)(currentTick - previousTick)
        );

        previousTick = currentTick;

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------

void app_main(void)
{
    printf("Tick interrupt demonstration started\n");

    printf(
        "Tick period = %lu ms\n",
        (unsigned long)portTICK_PERIOD_MS
    );

    xTaskCreatePinnedToCore(
        tickTask,
        "TickTask",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        NULL,
        0
    );

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

//---------------------------------------------------



