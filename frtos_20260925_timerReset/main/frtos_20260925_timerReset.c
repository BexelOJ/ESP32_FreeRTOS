#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

//---------------------------------------------------

#define TIMER_PERIOD_MS    5000

//---------------------------------------------------

static TimerHandle_t resetTimer;

//---------------------------------------------------

static void timerCallback(TimerHandle_t timer)
{
    printf("Timer expired\n");
}

//---------------------------------------------------

static void timerControlTask(void *pvParameters)
{
    //---------------------------------------------------
    // Start the timer
    //---------------------------------------------------

    printf("Starting timer\n");

    if (xTimerStart(
            resetTimer,
            portMAX_DELAY) != pdPASS)
    {
        printf("Failed to start timer\n");
    }

    //---------------------------------------------------
    // Wait for 3 seconds
    //---------------------------------------------------

    vTaskDelay(pdMS_TO_TICKS(3000));

    //---------------------------------------------------
    // Reset timer
    //---------------------------------------------------

    printf("Resetting timer after 3 seconds\n");

    if (xTimerReset(
            resetTimer,
            portMAX_DELAY) != pdPASS)
    {
        printf("Failed to reset timer\n");
    }

    //---------------------------------------------------
    // Timer now has a fresh 5-second countdown
    //---------------------------------------------------

    vTaskDelay(pdMS_TO_TICKS(3000));

    //---------------------------------------------------
    // Reset again
    //---------------------------------------------------

    printf("Resetting timer again after 3 seconds\n");

    if (xTimerReset(
            resetTimer,
            portMAX_DELAY) != pdPASS)
    {
        printf("Failed to reset timer\n");
    }

    //---------------------------------------------------
    // Allow timer to finally expire
    //---------------------------------------------------

    vTaskDelay(pdMS_TO_TICKS(6000));

    //---------------------------------------------------

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------

void app_main(void)
{
    //---------------------------------------------------
    // Create one-shot timer
    //---------------------------------------------------

    resetTimer = xTimerCreate(
        "ResetTimer",
        pdMS_TO_TICKS(TIMER_PERIOD_MS),
        pdFALSE,
        NULL,
        timerCallback
    );

    if (resetTimer == NULL)
    {
        printf("Failed to create timer\n");
        return;
    }

    //---------------------------------------------------
    // Create control task
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

    printf("Timer reset demonstration started\n");

    //---------------------------------------------------

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------



