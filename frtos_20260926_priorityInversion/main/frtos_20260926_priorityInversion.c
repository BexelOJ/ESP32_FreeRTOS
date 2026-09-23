#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

//---------------------------------------------------

#define LOW_TASK_PRIORITY       2
#define MEDIUM_TASK_PRIORITY    3
#define HIGH_TASK_PRIORITY      5

#define TEST_CORE               0

//---------------------------------------------------

static SemaphoreHandle_t resourceMutex;

//---------------------------------------------------

static void lowPriorityTask(void *pvParameters)
{
    while (1)
    {
        //---------------------------------------------------
        // Take shared resource
        //---------------------------------------------------

        printf("\nLow-priority task: taking mutex\n");

        xSemaphoreTake(
            resourceMutex,
            portMAX_DELAY
        );

        printf("Low-priority task: mutex acquired\n");

        //---------------------------------------------------
        // Hold resource
        //---------------------------------------------------

        printf("Low-priority task: using shared resource\n");

        for (volatile int i = 0; i < 5000000; i++)
        {
        }

        //---------------------------------------------------
        // Release resource
        //---------------------------------------------------

        printf("Low-priority task: releasing mutex\n");

        xSemaphoreGive(resourceMutex);

        //---------------------------------------------------
        // Give other tasks an opportunity to run
        //---------------------------------------------------

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------

static void mediumPriorityTask(void *pvParameters)
{
    while (1)
    {
        //---------------------------------------------------
        // CPU-intensive work
        //---------------------------------------------------

        printf("Medium-priority task running\n");

        for (volatile int i = 0; i < 3000000; i++)
        {
        }

        //---------------------------------------------------

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

//---------------------------------------------------

static void highPriorityTask(void *pvParameters)
{
    //---------------------------------------------------
    // Wait for the low-priority task to acquire mutex
    //---------------------------------------------------

    vTaskDelay(pdMS_TO_TICKS(500));

    //---------------------------------------------------

    while (1)
    {
        //---------------------------------------------------
        // Try to acquire shared resource
        //---------------------------------------------------

        printf("\nHigh-priority task: requesting mutex\n");

        if (xSemaphoreTake(
                resourceMutex,
                portMAX_DELAY) == pdTRUE)
        {
            printf("High-priority task: mutex acquired\n");

            //---------------------------------------------------
            // Use shared resource
            //---------------------------------------------------

            printf("High-priority task: using shared resource\n");

            //---------------------------------------------------

            xSemaphoreGive(resourceMutex);

            printf("High-priority task: mutex released\n");
        }

        //---------------------------------------------------

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

//---------------------------------------------------

void app_main(void)
{
    //---------------------------------------------------
    // Create mutex
    //---------------------------------------------------

    resourceMutex = xSemaphoreCreateMutex();

    if (resourceMutex == NULL)
    {
        printf("Failed to create mutex\n");
        return;
    }

    //---------------------------------------------------
    // Create low-priority task
    //---------------------------------------------------

    xTaskCreatePinnedToCore(
        lowPriorityTask,
        "LowPriorityTask",
        4096,
        NULL,
        LOW_TASK_PRIORITY,
        NULL,
        TEST_CORE
    );

    //---------------------------------------------------
    // Create medium-priority task
    //---------------------------------------------------

    xTaskCreatePinnedToCore(
        mediumPriorityTask,
        "MediumPriorityTask",
        4096,
        NULL,
        MEDIUM_TASK_PRIORITY,
        NULL,
        TEST_CORE
    );

    //---------------------------------------------------
    // Create high-priority task
    //---------------------------------------------------

    xTaskCreatePinnedToCore(
        highPriorityTask,
        "HighPriorityTask",
        4096,
        NULL,
        HIGH_TASK_PRIORITY,
        NULL,
        TEST_CORE
    );

    //---------------------------------------------------

    printf("Priority inversion demonstration started\n");

    //---------------------------------------------------

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------



