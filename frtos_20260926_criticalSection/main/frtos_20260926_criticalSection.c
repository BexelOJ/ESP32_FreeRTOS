#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//---------------------------------------------------

#define TASK_PRIORITY      5
#define TEST_CORE          0
#define INCREMENT_COUNT    100000

//---------------------------------------------------

static int sharedCounter = 0;

//---------------------------------------------------

static portMUX_TYPE counterSpinlock = portMUX_INITIALIZER_UNLOCKED;

//---------------------------------------------------

static void incrementTask(void *pvParameters)
{
    const char *taskName = (const char *)pvParameters;

    //---------------------------------------------------

    for (int i = 0; i < INCREMENT_COUNT; i++)
    {
        //---------------------------------------------------
        // Enter critical section
        //---------------------------------------------------

        portENTER_CRITICAL(&counterSpinlock);

        //---------------------------------------------------
        // Protected operation
        //---------------------------------------------------

        sharedCounter++;

        //---------------------------------------------------
        // Leave critical section
        //---------------------------------------------------

        portEXIT_CRITICAL(&counterSpinlock);
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
    // Initialize shared counter
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
    // Wait for both tasks
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



