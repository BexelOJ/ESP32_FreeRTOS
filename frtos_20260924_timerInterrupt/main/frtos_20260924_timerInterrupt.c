#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gptimer.h"
#include "esp_err.h"

//---------------------------------------------------

#define TIMER_FREQUENCY_HZ    1000000
#define TIMER_PERIOD_US       1000000

//---------------------------------------------------

static TaskHandle_t timerTaskHandle = NULL;
static gptimer_handle_t timerHandle = NULL;

//---------------------------------------------------

static bool IRAM_ATTR timerISRHandler(
    gptimer_handle_t timer,
    const gptimer_alarm_event_data_t *edata,
    void *user_ctx)
{
    BaseType_t higherPriorityTaskWoken = pdFALSE;

    vTaskNotifyGiveFromISR(
        timerTaskHandle,
        &higherPriorityTaskWoken
    );

    return higherPriorityTaskWoken == pdTRUE;
}

//---------------------------------------------------

static void timerTask(void *pvParameters)
{
    int counter = 0;

    while (1)
    {
        ulTaskNotifyTake(
            pdTRUE,
            portMAX_DELAY
        );

        counter++;

        printf(
            "Timer interrupt received: %d second(s)\n",
            counter
        );
    }
}

//---------------------------------------------------

void app_main(void)
{
    //---------------------------------------------------
    // Create FreeRTOS task
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
    // Configure GPTimer
    //---------------------------------------------------

    gptimer_config_t timerConfig =
    {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = TIMER_FREQUENCY_HZ
    };

    ESP_ERROR_CHECK(
        gptimer_new_timer(
            &timerConfig,
            &timerHandle
        )
    );

    //---------------------------------------------------
    // Configure alarm
    //---------------------------------------------------

    gptimer_alarm_config_t alarmConfig =
    {
        .reload_count = 0,
        .alarm_count = TIMER_PERIOD_US,
        .flags.auto_reload_on_alarm = true
    };

    ESP_ERROR_CHECK(
        gptimer_set_alarm_action(
            timerHandle,
            &alarmConfig
        )
    );

    //---------------------------------------------------
    // Register ISR callback
    //---------------------------------------------------

    gptimer_event_callbacks_t callbacks =
    {
        .on_alarm = timerISRHandler
    };

    ESP_ERROR_CHECK(
        gptimer_register_event_callbacks(
            timerHandle,
            &callbacks,
            NULL
        )
    );

    //---------------------------------------------------
    // Enable timer
    //---------------------------------------------------

    ESP_ERROR_CHECK(
        gptimer_enable(timerHandle)
    );

    //---------------------------------------------------
    // Start timer
    //---------------------------------------------------

    ESP_ERROR_CHECK(
        gptimer_start(timerHandle)
    );

    //---------------------------------------------------

    printf("Hardware timer interrupt started\n");
    printf("Interrupt period: 1 second\n");

    //---------------------------------------------------

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------



