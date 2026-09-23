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
    printf("Controller Task: Waiting before deleting Worker Task\n");

    vTaskDelay(pdMS_TO_TICKS(5000));

    printf("Controller Task: Deleting Worker Task\n");

    vTaskDelete(workerTaskHandle);

    printf("Controller Task: Worker Task deleted\n");

    while (1)
    {
        printf("Controller Task: Still running\n");

        vTaskDelay(pdMS_TO_TICKS(2000));
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
        controllerTask,
        "ControllerTask",
        2048,
        NULL,
        1,
        NULL,
        1
    );
}



