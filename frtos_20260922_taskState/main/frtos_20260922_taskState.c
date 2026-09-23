#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//---------------------------------------------------
static TaskHandle_t workerTaskHandle = NULL;

//---------------------------------------------------
static const char *getTaskStateName(eTaskState state)
{
    switch (state)
    {
        case eRunning:
            return "Running";

        case eReady:
            return "Ready";

        case eBlocked:
            return "Blocked";

        case eSuspended:
            return "Suspended";

        case eDeleted:
            return "Deleted";

        case eInvalid:
            return "Invalid";

        default:
            return "Unknown";
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
static void monitorTask(void *pvParameters)
{
    while (1)
    {
        eTaskState state = eTaskGetState(workerTaskHandle);

        printf(
            "Monitor Task: Worker State = %s\n",
            getTaskStateName(state)
        );

        vTaskDelay(pdMS_TO_TICKS(500));
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
        monitorTask,
        "MonitorTask",
        2048,
        NULL,
        2,
        NULL
    );
}



