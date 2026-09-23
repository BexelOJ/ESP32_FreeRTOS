#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//---------------------------------------------------
#define TASK_PERIOD_MS 1000

//---------------------------------------------------
static void periodicTask(void *pvParameters)
{
    TickType_t lastWakeTime;

    lastWakeTime = xTaskGetTickCount();

    int counter = 0;

    while (1)
    {
        printf(
            "Periodic Task: Running - Counter = %d\n",
            counter++
        );

        vTaskDelayUntil(
            &lastWakeTime,
            pdMS_TO_TICKS(TASK_PERIOD_MS)
        );
    }
}

//---------------------------------------------------
void app_main(void)
{
    xTaskCreatePinnedToCore(
        periodicTask,
        "PeriodicTask",
        2048,
        NULL,
        1,
        NULL,
        0
    );
}



