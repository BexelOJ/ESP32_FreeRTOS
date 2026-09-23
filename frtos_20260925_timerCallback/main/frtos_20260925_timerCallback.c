#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

//---------------------------------------------------

#define TIMER_1_PERIOD_MS    1000
#define TIMER_2_PERIOD_MS    3000

//---------------------------------------------------

static TimerHandle_t timer1;
static TimerHandle_t timer2;

//---------------------------------------------------

static void timerCallback(TimerHandle_t timer)
{
    const char *timerName;
    int timerNumber;

    //---------------------------------------------------
    // Get timer name
    //---------------------------------------------------

    timerName = pcTimerGetName(timer);

    //---------------------------------------------------
    // Get timer ID
    //---------------------------------------------------

    timerNumber = (int)(intptr_t)pvTimerGetTimerID(timer);

    //---------------------------------------------------

    printf(
        "Timer callback executed\n"
        "Name   : %s\n"
        "Number : %d\n",
        timerName,
        timerNumber
    );
}

//---------------------------------------------------

void app_main(void)
{
    //---------------------------------------------------
    // Create Timer 1
    //---------------------------------------------------

    timer1 = xTimerCreate(
        "Timer1",
        pdMS_TO_TICKS(TIMER_1_PERIOD_MS),
        pdTRUE,
        (void *)(intptr_t)1,
        timerCallback
    );

    //---------------------------------------------------
    // Create Timer 2
    //---------------------------------------------------

    timer2 = xTimerCreate(
        "Timer2",
        pdMS_TO_TICKS(TIMER_2_PERIOD_MS),
        pdTRUE,
        (void *)(intptr_t)2,
        timerCallback
    );

    //---------------------------------------------------

    if (timer1 == NULL || timer2 == NULL)
    {
        printf("Failed to create timers\n");
        return;
    }

    //---------------------------------------------------
    // Start Timer 1
    //---------------------------------------------------

    if (xTimerStart(timer1, 0) != pdPASS)
    {
        printf("Failed to start Timer 1\n");
        return;
    }

    //---------------------------------------------------
    // Start Timer 2
    //---------------------------------------------------

    if (xTimerStart(timer2, 0) != pdPASS)
    {
        printf("Failed to start Timer 2\n");
        return;
    }

    //---------------------------------------------------

    printf("Timer callback demonstration started\n");

    //---------------------------------------------------

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------



