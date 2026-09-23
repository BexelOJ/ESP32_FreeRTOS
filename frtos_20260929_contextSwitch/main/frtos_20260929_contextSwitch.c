#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//---------------------------------------------------

#define TASK_STACK_SIZE    4096
#define TASK_PRIORITY      5

//---------------------------------------------------

static TaskHandle_t taskAHandle;
static TaskHandle_t taskBHandle;

//---------------------------------------------------

static void taskA(void *pvParameters)
{
    while (1)
    {
        printf(
            "[TASK A] Running | Core = %d\n",
            xPortGetCoreID()
        );

        printf("[TASK A] Requesting context switch\n");

        taskYIELD();
    }
}

//---------------------------------------------------

static void taskB(void *pvParameters)
{
    while (1)
    {
        printf(
            "[TASK B] Running | Core = %d\n",
            xPortGetCoreID()
        );

        printf("[TASK B] Requesting context switch\n");

        taskYIELD();
    }
}

//---------------------------------------------------

void app_main(void)
{
    xTaskCreatePinnedToCore(
        taskA,
        "TaskA",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        &taskAHandle,
        0
    );

    xTaskCreatePinnedToCore(
        taskB,
        "TaskB",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        NULL,
        0
    );

    printf("Context switch demonstration started\n");

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------



