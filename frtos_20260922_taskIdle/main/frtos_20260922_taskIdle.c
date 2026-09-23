#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

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
static void monitorTask(void *pvParameters)
{
    while (1)
    {
        printf("Monitor Task: Running\n");

        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}

//---------------------------------------------------
static void idleMonitorTask(void *pvParameters)
{
    while (1)
    {
        printf("Idle Monitor Task: Running\n");

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

//---------------------------------------------------
void app_main(void)
{
    //---------------------------------------------------
    xTaskCreatePinnedToCore(
        workerTask,
        "WorkerTask",
        2048,
        NULL,
        1,
        NULL,
        0
    );

    //---------------------------------------------------
    xTaskCreatePinnedToCore(
        monitorTask,
        "MonitorTask",
        2048,
        NULL,
        1,
        NULL,
        0
    );

    //---------------------------------------------------
    xTaskCreatePinnedToCore(
        idleMonitorTask,
        "IdleMonitorTask",
        2048,
        NULL,
        0,
        NULL,
        0
    );
}



