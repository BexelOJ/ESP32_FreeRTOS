#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//-------------------------------------------
//---------------------------------------------------
void task1(void *pvParameters)
{
    while (1)
    {
        printf("Task 1 running\n");

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//-------------------------------------------

void task2(void *pvParameters)
{
    while (1)
    {
        printf("Task 2 running\n");

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//-------------------------------------------

void app_main(void)
{
    xTaskCreatePinnedToCore(
        task1,
        "Task1",
        4096,
        NULL,
        3,
        NULL,
        0
    );

    xTaskCreatePinnedToCore(
        task2,
        "Task2",
        4096,
        NULL,
        6,
        NULL,
        1
    );
}


//----------------------------------------------------


