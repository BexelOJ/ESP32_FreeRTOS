#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

//---------------------------------------------------

#define TIMER_PERIOD_MS    1000

//---------------------------------------------------

static TimerHandle_t periodicTimer;

//---------------------------------------------------

static void timerCallback(TimerHandle_t timer)
{
    static int counter = 0;

    counter++;

    printf(
        "Timer callback: %d\n",
        counter
    );
}

//---------------------------------------------------

static void timerControlTask(void *pvParameters)
{
    //---------------------------------------------------
    // Wait before starting timer
    //---------------------------------------------------

    vTaskDelay(pdMS_TO_TICKS(2000));

    printf("Starting software timer\n");

    if (xTimerStart(
            periodicTimer,
            portMAX_DELAY) != pdPASS)
    {
        printf("Failed to start timer\n");
    }

    //---------------------------------------------------
    // Allow timer to run
    //---------------------------------------------------

    vTaskDelay(pdMS_TO_TICKS(5000));

    //---------------------------------------------------
    // Stop timer
    //---------------------------------------------------

    printf("Stopping software timer\n");

    if (xTimerStop(
            periodicTimer,
            portMAX_DELAY) != pdPASS)
    {
        printf("Failed to stop timer\n");
    }

    //---------------------------------------------------
    // Timer is now stopped
    //---------------------------------------------------

    printf("Timer stopped\n");

    //---------------------------------------------------

    while (1)
    {
        printf("Timer remains stopped\n");

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

//---------------------------------------------------

void app_main(void)
{
    //---------------------------------------------------
    // Create periodic software timer
    //---------------------------------------------------

    periodicTimer = xTimerCreate(
        "PeriodicTimer",
        pdMS_TO_TICKS(TIMER_PERIOD_MS),
        pdTRUE,
        NULL,
        timerCallback
    );

    if (periodicTimer == NULL)
    {
        printf("Failed to create timer\n");
        return;
    }

    //---------------------------------------------------
    // Create timer control task
    //---------------------------------------------------

    xTaskCreate(
        timerControlTask,
        "TimerControlTask",
        4096,
        NULL,
        2,
        NULL
    );

    //---------------------------------------------------

    printf("Timer start/stop demonstration started\n");

    //---------------------------------------------------

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------



