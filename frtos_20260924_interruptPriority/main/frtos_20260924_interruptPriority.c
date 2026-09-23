#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gptimer.h"
#include "esp_err.h"

//---------------------------------------------------

#define TIMER_FREQUENCY_HZ    1000000

#define TIMER_1_PERIOD_US     2000000
#define TIMER_2_PERIOD_US     3000000

//---------------------------------------------------

static TaskHandle_t timer1TaskHandle = NULL;
static TaskHandle_t timer2TaskHandle = NULL;

static gptimer_handle_t timer1Handle = NULL;
static gptimer_handle_t timer2Handle = NULL;

//---------------------------------------------------

static bool IRAM_ATTR timer1ISR(
    gptimer_handle_t timer,
    const gptimer_alarm_event_data_t *edata,
    void *user_ctx)
{
    BaseType_t higherPriorityTaskWoken = pdFALSE;

    vTaskNotifyGiveFromISR(
        timer1TaskHandle,
        &higherPriorityTaskWoken
    );

    return higherPriorityTaskWoken == pdTRUE;
}

//---------------------------------------------------

static bool IRAM_ATTR timer2ISR(
    gptimer_handle_t timer,
    const gptimer_alarm_event_data_t *edata,
    void *user_ctx)
{
    BaseType_t higherPriorityTaskWoken = pdFALSE;

    vTaskNotifyGiveFromISR(
        timer2TaskHandle,
        &higherPriorityTaskWoken
    );

    return higherPriorityTaskWoken == pdTRUE;
}

//---------------------------------------------------

static void timer1Task(void *pvParameters)
{
    while (1)
    {
        ulTaskNotifyTake(
            pdTRUE,
            portMAX_DELAY
        );

        printf(
            "Timer 1 ISR executed - interrupt priority = 1\n"
        );
    }
}

//---------------------------------------------------

static void timer2Task(void *pvParameters)
{
    while (1)
    {
        ulTaskNotifyTake(
            pdTRUE,
            portMAX_DELAY
        );

        printf(
            "Timer 2 ISR executed - interrupt priority = 3\n"
        );
    }
}

//---------------------------------------------------

static void createTimer1(void)
{
    gptimer_config_t timerConfig =
    {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = TIMER_FREQUENCY_HZ,
        .intr_priority = 1
    };

    ESP_ERROR_CHECK(
        gptimer_new_timer(
            &timerConfig,
            &timer1Handle
        )
    );

    //---------------------------------------------------

    gptimer_alarm_config_t alarmConfig =
    {
        .reload_count = 0,
        .alarm_count = TIMER_1_PERIOD_US,
        .flags.auto_reload_on_alarm = true
    };

    ESP_ERROR_CHECK(
        gptimer_set_alarm_action(
            timer1Handle,
            &alarmConfig
        )
    );

    //---------------------------------------------------

    gptimer_event_callbacks_t callbacks =
    {
        .on_alarm = timer1ISR
    };

    ESP_ERROR_CHECK(
        gptimer_register_event_callbacks(
            timer1Handle,
            &callbacks,
            NULL
        )
    );

    //---------------------------------------------------

    ESP_ERROR_CHECK(
        gptimer_enable(timer1Handle)
    );

    ESP_ERROR_CHECK(
        gptimer_start(timer1Handle)
    );
}

//---------------------------------------------------

static void createTimer2(void)
{
    gptimer_config_t timerConfig =
    {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = TIMER_FREQUENCY_HZ,
        .intr_priority = 3
    };

    ESP_ERROR_CHECK(
        gptimer_new_timer(
            &timerConfig,
            &timer2Handle
        )
    );

    //---------------------------------------------------

    gptimer_alarm_config_t alarmConfig =
    {
        .reload_count = 0,
        .alarm_count = TIMER_2_PERIOD_US,
        .flags.auto_reload_on_alarm = true
    };

    ESP_ERROR_CHECK(
        gptimer_set_alarm_action(
            timer2Handle,
            &alarmConfig
        )
    );

    //---------------------------------------------------

    gptimer_event_callbacks_t callbacks =
    {
        .on_alarm = timer2ISR
    };

    ESP_ERROR_CHECK(
        gptimer_register_event_callbacks(
            timer2Handle,
            &callbacks,
            NULL
        )
    );

    //---------------------------------------------------

    ESP_ERROR_CHECK(
        gptimer_enable(timer2Handle)
    );

    ESP_ERROR_CHECK(
        gptimer_start(timer2Handle)
    );
}

//---------------------------------------------------

void app_main(void)
{
    //---------------------------------------------------
    // Create tasks
    //---------------------------------------------------

    xTaskCreate(
        timer1Task,
        "Timer1Task",
        4096,
        NULL,
        2,
        &timer1TaskHandle
    );

    //---------------------------------------------------

    xTaskCreate(
        timer2Task,
        "Timer2Task",
        4096,
        NULL,
        2,
        &timer2TaskHandle
    );

    //---------------------------------------------------
    // Create hardware timers
    //---------------------------------------------------

    createTimer1();

    createTimer2();

    //---------------------------------------------------

    printf("Interrupt priority demonstration started\n");
    printf("Timer 1 interrupt priority = 1\n");
    printf("Timer 2 interrupt priority = 3\n");

    //---------------------------------------------------

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------



