#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//---------------------------------------------------
static void taskA(void *pvParameters)
{
    int counter = 0;

    while (1)
    {
        printf("Task A: Running - Counter = %d\n", counter++);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------
static void taskB(void *pvParameters)
{
    int counter = 0;

    while (1)
    {
        printf("Task B: Running - Counter = %d\n", counter++);

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

//---------------------------------------------------
void app_main(void)
{
    //---------------------------------------------------
    xTaskCreatePinnedToCore(
        taskA,
        "TaskA",
        2048,
        NULL,
        1,
        NULL,
        0
    );

    //---------------------------------------------------
    xTaskCreatePinnedToCore(
        taskB,
        "TaskB",
        2048,
        NULL,
        1,
        NULL,
        0
    );
}



