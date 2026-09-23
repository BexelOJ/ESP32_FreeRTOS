#include <stdio.h>
#include <stdint.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

//---------------------------------------------------

#define TIMER_1_PERIOD_MS    1000
#define TIMER_2_PERIOD_MS    2000
#define TIMER_3_PERIOD_MS    5000

//---------------------------------------------------

static TimerHandle_t timer1;
static TimerHandle_t timer2;
static TimerHandle_t timer3;

//---------------------------------------------------

static void timerCallback(TimerHandle_t timer)
{
    int timerNumber;

    //---------------------------------------------------
    // Get timer ID
    //---------------------------------------------------

    timerNumber = (int)(intptr_t)pvTimerGetTimerID(timer);

    //---------------------------------------------------

    printf(
        "Timer callback: Timer %d (%s)\n",
        timerNumber,
        pcTimerGetName(timer)
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
    // Create Timer 3
    //---------------------------------------------------

    timer3 = xTimerCreate(
        "Timer3",
        pdMS_TO_TICKS(TIMER_3_PERIOD_MS),
        pdTRUE,
        (void *)(intptr_t)3,
        timerCallback
    );

    //---------------------------------------------------
    // Check timer creation
    //---------------------------------------------------

    if (timer1 == NULL ||
        timer2 == NULL ||
        timer3 == NULL)
    {
        printf("Failed to create one or more timers\n");
        return;
    }

    //---------------------------------------------------
    // Start Timer 1
    //---------------------------------------------------

    if (xTimerStart(timer1, 0) != pdPASS)
    {
        printf("Failed to start Timer 1\n");
    }

    //---------------------------------------------------
    // Start Timer 2
    //---------------------------------------------------

    if (xTimerStart(timer2, 0) != pdPASS)
    {
        printf("Failed to start Timer 2\n");
    }

    //---------------------------------------------------
    // Start Timer 3
    //---------------------------------------------------

    if (xTimerStart(timer3, 0) != pdPASS)
    {
        printf("Failed to start Timer 3\n");
    }

    //---------------------------------------------------

    printf("Multiple timer demonstration started\n");

    //---------------------------------------------------

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------



