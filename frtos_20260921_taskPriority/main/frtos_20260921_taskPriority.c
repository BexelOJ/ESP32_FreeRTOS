#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//---------------------------------------------------
static void lowPriorityTask(void *pvParameters)
{
    while (1)
    {
        printf("Low Priority Task: Running\n");

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------
static void mediumPriorityTask(void *pvParameters)
{
    while (1)
    {
        printf("Medium Priority Task: Running\n");

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------
static void highPriorityTask(void *pvParameters)
{
    while (1)
    {
        printf("High Priority Task: Running\n");

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------
void app_main(void)
{
    //---------------------------------------------------
    xTaskCreatePinnedToCore(
        lowPriorityTask,
        "LowPriorityTask",
        2048,
        NULL,
        1,
        NULL,
        0
    );

    //---------------------------------------------------
    xTaskCreatePinnedToCore(
        mediumPriorityTask,
        "MediumPriorityTask",
        2048,
        NULL,
        2,
        NULL,
        0
    );

    //---------------------------------------------------
    xTaskCreatePinnedToCore(
        highPriorityTask,
        "HighPriorityTask",
        2048,
        NULL,
        3,
        NULL,
        0
    );
}





