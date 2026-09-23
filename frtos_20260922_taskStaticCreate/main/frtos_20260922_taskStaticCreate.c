#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//---------------------------------------------------
#define TASK_STACK_SIZE 2048

//---------------------------------------------------
static StaticTask_t taskOneTCB;
static StackType_t taskOneStack[TASK_STACK_SIZE];

//---------------------------------------------------
static StaticTask_t taskTwoTCB;
static StackType_t taskTwoStack[TASK_STACK_SIZE];

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
    TaskHandle_t taskOneHandle;
    TaskHandle_t taskTwoHandle;

    //---------------------------------------------------
    taskOneHandle = xTaskCreateStatic(
        taskOne,
        "TaskOne",
        TASK_STACK_SIZE,
        NULL,
        1,
        taskOneStack,
        &taskOneTCB
    );

    //---------------------------------------------------
    if (taskOneHandle != NULL)
    {
        printf("Task One created successfully\n");
    }
    else
    {
        printf("Task One creation failed\n");
    }

    //---------------------------------------------------
    taskTwoHandle = xTaskCreateStatic(
        taskTwo,
        "TaskTwo",
        TASK_STACK_SIZE,
        NULL,
        1,
        taskTwoStack,
        &taskTwoTCB
    );

    //---------------------------------------------------
    if (taskTwoHandle != NULL)
    {
        printf("Task Two created successfully\n");
    }
    else
    {
        printf("Task Two creation failed\n");
    }
}



