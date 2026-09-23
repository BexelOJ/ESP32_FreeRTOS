#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

//---------------------------------------------------

#define TASK_PRIORITY      5
#define TASK_STACK_SIZE    4096

//---------------------------------------------------

static SemaphoreHandle_t counterMutex;

static volatile int sharedCounter = 0;

//---------------------------------------------------

static void core0Task(void *pvParameters)
{
    while (1)
    {
        if (xSemaphoreTake(
                counterMutex,
                portMAX_DELAY) == pdTRUE)
        {
            sharedCounter++;

            printf(
                "Core 0: sharedCounter = %d\n",
                sharedCounter
            );

            xSemaphoreGive(counterMutex);
        }

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

//---------------------------------------------------

static void core1Task(void *pvParameters)
{
    while (1)
    {
        if (xSemaphoreTake(
                counterMutex,
                portMAX_DELAY) == pdTRUE)
        {
            sharedCounter++;

            printf(
                "Core 1: sharedCounter = %d\n",
                sharedCounter
            );

            xSemaphoreGive(counterMutex);
        }

        vTaskDelay(pdMS_TO_TICKS(700));
    }
}

//---------------------------------------------------

void app_main(void)
{
    counterMutex = xSemaphoreCreateMutex();

    if (counterMutex == NULL)
    {
        printf("Failed to create mutex\n");
        return;
    }

    //---------------------------------------------------

    xTaskCreatePinnedToCore(
        core0Task,
        "Core0Task",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        NULL,
        0
    );

    xTaskCreatePinnedToCore(
        core1Task,
        "Core1Task",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        NULL,
        1
    );

    printf("Multicore synchronization demonstration started\n");

    //---------------------------------------------------
}



