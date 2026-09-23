#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

//---------------------------------------------------

#define TIMER_PERIOD_MS    2000

//---------------------------------------------------

static TimerHandle_t periodicTimer;

//---------------------------------------------------

static void periodicTimerCallback(TimerHandle_t timer)
{
    static int counter = 0;

    counter++;

    printf(
        "Periodic Timer Callback: %d\n",
        counter
    );
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
        periodicTimerCallback
    );

    if (periodicTimer == NULL)
    {
        printf("Failed to create periodic timer\n");
        return;
    }

    //---------------------------------------------------
    // Start timer
    //---------------------------------------------------

    if (xTimerStart(
            periodicTimer,
            0) != pdPASS)
    {
        printf("Failed to start periodic timer\n");
        return;
    }

    //---------------------------------------------------

    printf("Periodic timer started\n");
    printf("Period: %d ms\n", TIMER_PERIOD_MS);

    //---------------------------------------------------

    while (1)
    {
        printf("app_main is running\n");

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------



