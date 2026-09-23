#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//---------------------------------------------------

#define TASK_PRIORITY    5

//---------------------------------------------------

static void taskOne(void *pvParameters)
{
    while (1)
    {
        printf("Task 1 running\n");

        for (volatile int i = 0; i < 1000000; i++)
        {
        }
    }
}

//---------------------------------------------------

static void taskTwo(void *pvParameters)
{
    while (1)
    {
        printf("Task 2 running\n");

        for (volatile int i = 0; i < 1000000; i++)
        {
        }
    }
}

//---------------------------------------------------

static void taskThree(void *pvParameters)
{
    while (1)
    {
        printf("Task 3 running\n");

        for (volatile int i = 0; i < 1000000; i++)
        {
        }
    }
}

//---------------------------------------------------

void app_main(void)
{
    //---------------------------------------------------
    // Create Task 1
    //---------------------------------------------------

    xTaskCreate(
        taskOne,
        "Task1",
        4096,
        NULL,
        TASK_PRIORITY,
        NULL
    );

    //---------------------------------------------------
    // Create Task 2
    //---------------------------------------------------

    xTaskCreate(
        taskTwo,
        "Task2",
        4096,
        NULL,
        TASK_PRIORITY,
        NULL
    );

    //---------------------------------------------------
    // Create Task 3
    //---------------------------------------------------

    xTaskCreate(
        taskThree,
        "Task3",
        4096,
        NULL,
        TASK_PRIORITY,
        NULL
    );

    //---------------------------------------------------

    printf("Round-robin scheduling demonstration started\n");

    //---------------------------------------------------

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------



