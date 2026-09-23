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
        printf("Worker Task: Running - Counter = %d\n", counter++);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------
static void controllerTask(void *pvParameters)
{
    vTaskDelay(pdMS_TO_TICKS(5000));

    //---------------------------------------------------
    printf("Controller Task: Suspending Worker Task\n");

    vTaskSuspend(workerTaskHandle);

    //---------------------------------------------------
    vTaskDelay(pdMS_TO_TICKS(3000));

    //---------------------------------------------------
    printf("Controller Task: Resuming Worker Task\n");

    vTaskResume(workerTaskHandle);

    //---------------------------------------------------
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------
void app_main(void)
{
    //---------------------------------------------------
    xTaskCreate(
        workerTask,
        "WorkerTask",
        2048,
        NULL,
        1,
        &workerTaskHandle
    );

    //---------------------------------------------------
    xTaskCreate(
        controllerTask,
        "ControllerTask",
        2048,
        NULL,
        2,
        NULL
    );
}



