#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//---------------------------------------------------
static TaskHandle_t workerTaskHandle = NULL;

//---------------------------------------------------
static void workerTask(void *pvParameters)
{
    int counter = 0;

    while (1)
    {
        printf("Worker Task: Running... Counter = %d\n", counter++);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------
static void controlTask(void *pvParameters)
{
    while (1)
    {
        printf("\nController: Suspending Worker Task\n");

        vTaskSuspend(workerTaskHandle);

        vTaskDelay(pdMS_TO_TICKS(3000));

        printf("\nController: Resuming Worker Task\n");

        vTaskResume(workerTaskHandle);

        vTaskDelay(pdMS_TO_TICKS(3000));
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
        &workerTaskHandle,
        0
    );

    //---------------------------------------------------
    xTaskCreatePinnedToCore(
        controlTask,
        "ControlTask",
        2048,
        NULL,
        1,
        NULL,
        1
    );
}



