#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//---------------------------------------------------

#define TASK_PRIORITY    5
#define TEST_CORE        0

//---------------------------------------------------

static volatile int sharedCounter = 0;

//---------------------------------------------------

static void workerTask(void *pvParameters)
{
    const char *taskName = (const char *)pvParameters;

    //---------------------------------------------------

    while (1)
    {
        printf(
            "%s running, counter = %d\n",
            taskName,
            sharedCounter
        );

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

//---------------------------------------------------

static void schedulerTask(void *pvParameters)
{
    //---------------------------------------------------
    // Give worker tasks time to start
    //---------------------------------------------------

    vTaskDelay(pdMS_TO_TICKS(2000));

    //---------------------------------------------------

    printf("\n");
    printf("========================================\n");
    printf("Suspending scheduler\n");
    printf("========================================\n");

    //---------------------------------------------------
    // Stop task scheduling
    //---------------------------------------------------

    vTaskSuspendAll();

    //---------------------------------------------------
    // Perform a group of operations
    //---------------------------------------------------

    printf("Scheduler suspended\n");

    for (int i = 0; i < 5; i++)
    {
        sharedCounter++;

        printf(
            "Scheduler suspended: counter = %d\n",
            sharedCounter
        );

        //---------------------------------------------------
        // This delay does NOT switch to another task
        // while the scheduler is suspended.
        //---------------------------------------------------

        for (volatile int j = 0; j < 1000000; j++)
        {
        }
    }

    //---------------------------------------------------
    // Resume scheduler
    //---------------------------------------------------

    printf("Resuming scheduler\n");

    xTaskResumeAll();

    //---------------------------------------------------

    printf("Scheduler resumed\n");

    //---------------------------------------------------

    vTaskDelete(NULL);
}

//---------------------------------------------------

void app_main(void)
{
    //---------------------------------------------------
    // Create Worker Task 1
    //---------------------------------------------------

    xTaskCreatePinnedToCore(
        workerTask,
        "Worker1",
        4096,
        "Worker 1",
        TASK_PRIORITY,
        NULL,
        TEST_CORE
    );

    //---------------------------------------------------
    // Create Worker Task 2
    //---------------------------------------------------

    xTaskCreatePinnedToCore(
        workerTask,
        "Worker2",
        4096,
        "Worker 2",
        TASK_PRIORITY,
        NULL,
        TEST_CORE
    );

    //---------------------------------------------------
    // Create scheduler-control task
    //---------------------------------------------------

    xTaskCreatePinnedToCore(
        schedulerTask,
        "SchedulerTask",
        4096,
        NULL,
        TASK_PRIORITY + 1,
        NULL,
        TEST_CORE
    );

    //---------------------------------------------------

    printf("Scheduler suspension demonstration started\n");

    //---------------------------------------------------

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------



