#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

//---------------------------------------------------

#define TIMER_PERIOD_MS    5000

//---------------------------------------------------

static TimerHandle_t oneShotTimer;

//---------------------------------------------------

static void oneShotTimerCallback(TimerHandle_t timer)
{
    printf("One-shot timer expired\n");
    printf("Callback executed only once\n");
}

//---------------------------------------------------

void app_main(void)
{
    //---------------------------------------------------
    // Create one-shot timer
    //---------------------------------------------------

    oneShotTimer = xTimerCreate(
        "OneShotTimer",
        pdMS_TO_TICKS(TIMER_PERIOD_MS),
        pdFALSE,
        NULL,
        oneShotTimerCallback
    );

    if (oneShotTimer == NULL)
    {
        printf("Failed to create one-shot timer\n");
        return;
    }

    //---------------------------------------------------
    // Start timer
    //---------------------------------------------------

    if (xTimerStart(
            oneShotTimer,
            0) != pdPASS)
    {
        printf("Failed to start one-shot timer\n");
        return;
    }

    //---------------------------------------------------

    printf("One-shot timer started\n");
    printf("Timer will expire after %d ms\n", TIMER_PERIOD_MS);

    //---------------------------------------------------

    while (1)
    {
        printf("app_main is running\n");

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------



