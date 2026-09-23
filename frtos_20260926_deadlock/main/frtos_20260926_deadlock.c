#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

//---------------------------------------------------

#define TASK_PRIORITY    5
#define TEST_CORE        0

//---------------------------------------------------

static SemaphoreHandle_t mutexA;
static SemaphoreHandle_t mutexB;

//---------------------------------------------------

static void taskOne(void *pvParameters)
{
    while (1)
    {
        //---------------------------------------------------
        // Take Mutex A
        //---------------------------------------------------

        printf("Task 1: requesting Mutex A\n");

        xSemaphoreTake(
            mutexA,
            portMAX_DELAY
        );

        printf("Task 1: Mutex A acquired\n");

        //---------------------------------------------------
        // Give Task 2 time to acquire Mutex B
        //---------------------------------------------------

        vTaskDelay(pdMS_TO_TICKS(500));

        //---------------------------------------------------
        // Try to take Mutex B
        //---------------------------------------------------

        printf("Task 1: requesting Mutex B\n");

        xSemaphoreTake(
            mutexB,
            portMAX_DELAY
        );

        //---------------------------------------------------
        // This line will never be reached
        // in the deadlock situation.
        //---------------------------------------------------

        printf("Task 1: Mutex B acquired\n");

        //---------------------------------------------------

        xSemaphoreGive(mutexB);
        xSemaphoreGive(mutexA);

        //---------------------------------------------------

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------

static void taskTwo(void *pvParameters)
{
    //---------------------------------------------------
    // Give Task 1 a chance to acquire Mutex A
    //---------------------------------------------------

    vTaskDelay(pdMS_TO_TICKS(100));

    //---------------------------------------------------

    while (1)
    {
        //---------------------------------------------------
        // Take Mutex B
        //---------------------------------------------------

        printf("Task 2: requesting Mutex B\n");

        xSemaphoreTake(
            mutexB,
            portMAX_DELAY
        );

        printf("Task 2: Mutex B acquired\n");

        //---------------------------------------------------
        // Give Task 1 time to reach Mutex B
        //---------------------------------------------------

        vTaskDelay(pdMS_TO_TICKS(500));

        //---------------------------------------------------
        // Try to take Mutex A
        //---------------------------------------------------

        printf("Task 2: requesting Mutex A\n");

        xSemaphoreTake(
            mutexA,
            portMAX_DELAY
        );

        //---------------------------------------------------
        // This line will never be reached
        // in the deadlock situation.
        //---------------------------------------------------

        printf("Task 2: Mutex A acquired\n");

        //---------------------------------------------------

        xSemaphoreGive(mutexA);
        xSemaphoreGive(mutexB);

        //---------------------------------------------------

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------

void app_main(void)
{
    //---------------------------------------------------
    // Create Mutex A
    //---------------------------------------------------

    mutexA = xSemaphoreCreateMutex();

    //---------------------------------------------------
    // Create Mutex B
    //---------------------------------------------------

    mutexB = xSemaphoreCreateMutex();

    //---------------------------------------------------

    if (mutexA == NULL || mutexB == NULL)
    {
        printf("Failed to create mutexes\n");
        return;
    }

    //---------------------------------------------------
    // Create Task 1
    //---------------------------------------------------

    xTaskCreatePinnedToCore(
        taskOne,
        "Task1",
        4096,
        NULL,
        TASK_PRIORITY,
        NULL,
        TEST_CORE
    );

    //---------------------------------------------------
    // Create Task 2
    //---------------------------------------------------

    xTaskCreatePinnedToCore(
        taskTwo,
        "Task2",
        4096,
        NULL,
        TASK_PRIORITY,
        NULL,
        TEST_CORE
    );

    //---------------------------------------------------

    printf("Deadlock demonstration started\n");

    //---------------------------------------------------

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------



