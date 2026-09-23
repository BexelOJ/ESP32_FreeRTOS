#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

//---------------------------------------------------

#define TIMER_PERIOD_MS    1000

//---------------------------------------------------

static TimerHandle_t softwareTimer;

//---------------------------------------------------

static void softwareTimerCallback(TimerHandle_t timer)
{
    static int counter = 0;

    counter++;

    printf(
        "Software Timer Callback: %d second(s)\n",
        counter
    );
}

//---------------------------------------------------

void app_main(void)
{
    //---------------------------------------------------
    // Create software timer
    //---------------------------------------------------

    softwareTimer = xTimerCreate(
        "SoftwareTimer",
        pdMS_TO_TICKS(TIMER_PERIOD_MS),
        pdTRUE,
        NULL,
        softwareTimerCallback
    );

    if (softwareTimer == NULL)
    {
        printf("Failed to create software timer\n");
        return;
    }

    //---------------------------------------------------
    // Start software timer
    //---------------------------------------------------

    if (xTimerStart(
            softwareTimer,
            0) != pdPASS)
    {
        printf("Failed to start software timer\n");
        return;
    }

    //---------------------------------------------------

    printf("Software timer started\n");
    printf("Timer period: %d ms\n", TIMER_PERIOD_MS);

    //---------------------------------------------------

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(5000));

        printf("app_main is still running\n");
    }
}

//---------------------------------------------------



