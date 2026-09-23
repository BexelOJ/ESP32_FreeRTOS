#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//---------------------------------------------------
static void taskOne(void *pvParameters)
{
    while (1)
    {
        printf("Task One: Running\n");

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------
static void taskTwo(void *pvParameters)
{
    while (1)
    {
        printf("Task Two: Running\n");

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

//---------------------------------------------------
void app_main(void)
{
    BaseType_t result;

    //---------------------------------------------------
    result = xTaskCreate(
        taskOne,
        "TaskOne",
        2048,
        NULL,
        1,
        NULL
    );

    //---------------------------------------------------
    if (result == pdPASS)
    {
        printf("Task One created successfully\n");
    }
    else
    {
        printf("Task One creation failed\n");
    }

    //---------------------------------------------------
    result = xTaskCreate(
        taskTwo,
        "TaskTwo",
        2048,
        NULL,
        1,
        NULL
    );

    //---------------------------------------------------
    if (result == pdPASS)
    {
        printf("Task Two created successfully\n");
    }
    else
    {
        printf("Task Two creation failed\n");
    }
}



