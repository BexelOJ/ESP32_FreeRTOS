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

static TaskHandle_t lowTaskHandle;
static TaskHandle_t mediumTaskHandle;
static TaskHandle_t highTaskHandle;

//---------------------------------------------------

static void lowPriorityTask(void *pvParameters)
{
    while (1)
    {
        //---------------------------------------------------
        // Take mutex
        //---------------------------------------------------

        printf("\nLow task: requesting mutex\n");

        if (xSemaphoreTake(
                resourceMutex,
                portMAX_DELAY) == pdTRUE)
        {
            printf("Low task: mutex acquired\n");

            //---------------------------------------------------
            // Hold mutex for a while
            //---------------------------------------------------

            for (int i = 0; i < 5; i++)
            {
                printf(
                    "Low task: working %d/5\n",
                    i + 1
                );

                for (volatile int j = 0; j < 3000000; j++)
                {
                }

                //---------------------------------------------------
                // Display current effective priority
                //---------------------------------------------------

                printf(
                    "Low task: current priority = %lu\n",
                    (unsigned long)uxTaskPriorityGet(NULL)
                );
            }

            //---------------------------------------------------
            // Release mutex
            //---------------------------------------------------

            printf("Low task: releasing mutex\n");

            xSemaphoreGive(resourceMutex);
        }

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

        printf("Medium task: running\n");

        for (volatile int i = 0; i < 4000000; i++)
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
    // Allow low-priority task to acquire mutex first
    //---------------------------------------------------

    vTaskDelay(pdMS_TO_TICKS(300));

    //---------------------------------------------------

    printf("\nHigh task: requesting mutex\n");

    //---------------------------------------------------
    // This will block because Low owns the mutex
    //---------------------------------------------------

    if (xSemaphoreTake(
            resourceMutex,
            portMAX_DELAY) == pdTRUE)
    {
        printf("High task: mutex acquired\n");

        //---------------------------------------------------
        // Use shared resource
        //---------------------------------------------------

        printf("High task: using shared resource\n");

        //---------------------------------------------------

        xSemaphoreGive(resourceMutex);

        printf("High task: mutex released\n");
    }

    //---------------------------------------------------
    // Finish demonstration
    //---------------------------------------------------

    vTaskDelete(NULL);
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
        "LowTask",
        4096,
        NULL,
        LOW_TASK_PRIORITY,
        &lowTaskHandle,
        TEST_CORE
    );

    //---------------------------------------------------
    // Create medium-priority task
    //---------------------------------------------------

    xTaskCreatePinnedToCore(
        mediumPriorityTask,
        "MediumTask",
        4096,
        NULL,
        MEDIUM_TASK_PRIORITY,
        &mediumTaskHandle,
        TEST_CORE
    );

    //---------------------------------------------------
    // Create high-priority task
    //---------------------------------------------------

    xTaskCreatePinnedToCore(
        highPriorityTask,
        "HighTask",
        4096,
        NULL,
        HIGH_TASK_PRIORITY,
        &highTaskHandle,
        TEST_CORE
    );

    //---------------------------------------------------

    printf("Priority inheritance demonstration started\n");

    //---------------------------------------------------

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------



