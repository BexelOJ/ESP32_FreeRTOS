#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//---------------------------------------------------

#define LOW_TASK_PRIORITY     2
#define HIGH_TASK_PRIORITY    5

//---------------------------------------------------

static void lowPriorityTask(void *pvParameters)
{
    while (1)
    {
        printf("Low-priority task running\n");

        //---------------------------------------------------
        // Simulate CPU-intensive work
        //---------------------------------------------------

        for (volatile int i = 0; i < 1000000; i++)
        {
        }
    }
}

//---------------------------------------------------

static void highPriorityTask(void *pvParameters)
{
    //---------------------------------------------------
    // Keep the high-priority task blocked initially
    //---------------------------------------------------

    vTaskDelay(pdMS_TO_TICKS(3000));

    //---------------------------------------------------
    // This task now becomes Ready
    //---------------------------------------------------

    printf("\n");
    printf("========================================\n");
    printf("High-priority task is now READY\n");
    printf("High-priority task is running\n");
    printf("========================================\n");
    printf("\n");

    //---------------------------------------------------
    // Keep running
    //---------------------------------------------------

    while (1)
    {
        printf("High-priority task running\n");

        //---------------------------------------------------
        // Allow the scheduler to run
        //---------------------------------------------------

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------

void app_main(void)
{
    //---------------------------------------------------
    // Create low-priority task
    //---------------------------------------------------

    xTaskCreate(
        lowPriorityTask,
        "LowPriorityTask",
        4096,
        NULL,
        LOW_TASK_PRIORITY,
        NULL
    );

    //---------------------------------------------------
    // Create high-priority task
    //---------------------------------------------------

    xTaskCreate(
        highPriorityTask,
        "HighPriorityTask",
        4096,
        NULL,
        HIGH_TASK_PRIORITY,
        NULL
    );

    //---------------------------------------------------

    printf("Priority preemption demonstration started\n");

    //---------------------------------------------------

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------



