#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//---------------------------------------------------

#define TASK_PRIORITY    5
#define TASK_STACK_SIZE  4096

//---------------------------------------------------

static void taskA(void *pvParameters)
{
    while (1)
    {
        printf("Task A running on Core %d\n", xPortGetCoreID());

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------

static void taskB(void *pvParameters)
{
    while (1)
    {
        printf("Task B running on Core %d\n", xPortGetCoreID());

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------

void app_main(void)
{
    printf("FreeRTOS Port Layer demonstration started\n");

    xTaskCreate(
        taskA,
        "TaskA",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        NULL
    );

    xTaskCreate(
        taskB,
        "TaskB",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        NULL
    );

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

//---------------------------------------------------



