#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

//---------------------------------------------------

#define TIMER_PERIOD_MS    1000

//---------------------------------------------------

static TimerHandle_t softwareTimer;
static TaskHandle_t timerTaskHandle;

//---------------------------------------------------

static void timerCallback(TimerHandle_t timer)
{
    //---------------------------------------------------
    // Notify the timer task
    //---------------------------------------------------

    xTaskNotifyGive(timerTaskHandle);
}

//---------------------------------------------------

static void timerTask(void *pvParameters)
{
    int counter = 0;

    //---------------------------------------------------

    while (1)
    {
        //---------------------------------------------------
        // Wait for timer notification
        //---------------------------------------------------

        ulTaskNotifyTake(
            pdTRUE,
            portMAX_DELAY
        );

        //---------------------------------------------------
        // Timer event received
        //---------------------------------------------------

        counter++;

        printf(
            "Timer task executed: %d\n",
            counter
        );

        //---------------------------------------------------
        // Real processing can be performed here
        //---------------------------------------------------

        printf("Timer task is doing the actual work\n");
    }
}

//---------------------------------------------------

void app_main(void)
{
    //---------------------------------------------------
    // Create timer task
    //---------------------------------------------------

    xTaskCreate(
        timerTask,
        "TimerTask",
        4096,
        NULL,
        2,
        &timerTaskHandle
    );

    //---------------------------------------------------
    // Create periodic software timer
    //---------------------------------------------------

    softwareTimer = xTimerCreate(
        "SoftwareTimer",
        pdMS_TO_TICKS(TIMER_PERIOD_MS),
        pdTRUE,
        NULL,
        timerCallback
    );

    //---------------------------------------------------
    // Check timer creation
    //---------------------------------------------------

    if (softwareTimer == NULL)
    {
        printf("Failed to create timer\n");
        return;
    }

    //---------------------------------------------------
    // Start timer
    //---------------------------------------------------

    if (xTimerStart(
            softwareTimer,
            portMAX_DELAY) != pdPASS)
    {
        printf("Failed to start timer\n");
        return;
    }

    //---------------------------------------------------

    printf("Timer task demonstration started\n");

    //---------------------------------------------------

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------



