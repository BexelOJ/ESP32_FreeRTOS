#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_task_wdt.h"

//---------------------------------------------------
#define WATCHDOG_TIMEOUT_SECONDS 5

//---------------------------------------------------
static void healthyTask(void *pvParameters)
{
    while (1)
    {
        printf("Healthy Task: Running\n");

        //---------------------------------------------------
        esp_task_wdt_reset();

        //---------------------------------------------------
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------
static void workerTask(void *pvParameters)
{
    while (1)
    {
        printf("Worker Task: Running\n");

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

//---------------------------------------------------
void app_main(void)
{
    //---------------------------------------------------
    esp_task_wdt_config_t watchdogConfig =
    {
        .timeout_ms = WATCHDOG_TIMEOUT_SECONDS * 1000,
        .idle_core_mask = 0,
        .trigger_panic = false
    };

    //---------------------------------------------------
    esp_err_t result = esp_task_wdt_reconfigure(&watchdogConfig);

    if (result != ESP_OK)
    {
        printf("Watchdog reconfiguration failed: %s\n",
               esp_err_to_name(result));
    }

    //---------------------------------------------------
    result = esp_task_wdt_add(NULL);

    if (result == ESP_OK)
    {
        printf("app_main task added to watchdog\n");
    }
    else
    {
        printf("Failed to add app_main task to watchdog: %s\n",
               esp_err_to_name(result));
    }

    //---------------------------------------------------
    xTaskCreate(
        healthyTask,
        "HealthyTask",
        2048,
        NULL,
        1,
        NULL
    );

    //---------------------------------------------------
    xTaskCreate(
        workerTask,
        "WorkerTask",
        2048,
        NULL,
        1,
        NULL
    );

    //---------------------------------------------------
    while (1)
    {
        printf("app_main: Resetting watchdog\n");

        esp_task_wdt_reset();

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}



