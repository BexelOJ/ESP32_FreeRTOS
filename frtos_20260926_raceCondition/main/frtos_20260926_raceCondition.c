#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//---------------------------------------------------

#define TASK_PRIORITY    5
#define TEST_CORE        0

#define INCREMENT_COUNT  100000

//---------------------------------------------------

static volatile int sharedCounter = 0;

//---------------------------------------------------

static void incrementTask(void *pvParameters)
{
    const char *taskName = (const char *)pvParameters;

    //---------------------------------------------------

    for (int i = 0; i < INCREMENT_COUNT; i++)
    {
        //---------------------------------------------------
        // Read
        //---------------------------------------------------

        int value = sharedCounter;

        //---------------------------------------------------
        // Simulate interruption between read and write
        //---------------------------------------------------

        for (volatile int j = 0; j < 10; j++)
        {
        }

        //---------------------------------------------------
        // Write
        //---------------------------------------------------

        sharedCounter = value + 1;
    }

    //---------------------------------------------------

    printf(
        "%s finished\n",
        taskName
    );

    //---------------------------------------------------

    vTaskDelete(NULL);
}

//---------------------------------------------------

void app_main(void)
{
    //---------------------------------------------------
    // Start value
    //---------------------------------------------------

    sharedCounter = 0;

    //---------------------------------------------------
    // Create Task 1
    //---------------------------------------------------

    xTaskCreatePinnedToCore(
        incrementTask,
        "Task1",
        4096,
        "Task 1",
        TASK_PRIORITY,
        NULL,
        TEST_CORE
    );

    //---------------------------------------------------
    // Create Task 2
    //---------------------------------------------------

    xTaskCreatePinnedToCore(
        incrementTask,
        "Task2",
        4096,
        "Task 2",
        TASK_PRIORITY,
        NULL,
        TEST_CORE
    );

    //---------------------------------------------------
    // Wait for both tasks to finish
    //---------------------------------------------------

    vTaskDelay(pdMS_TO_TICKS(5000));

    //---------------------------------------------------

    printf("\n");
    printf("========================================\n");
    printf("Expected counter : %d\n", INCREMENT_COUNT * 2);
    printf("Actual counter   : %d\n", sharedCounter);
    printf("========================================\n");

    //---------------------------------------------------

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------



