#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//---------------------------------------------------
typedef struct
{
    const char *name;
    int number;
} TaskParameters;

//---------------------------------------------------
static void workerTask(void *pvParameters)
{
    TaskParameters *parameters = (TaskParameters *)pvParameters;

    while (1)
    {
        printf(
            "Task: %s | Number: %d\n",
            parameters->name,
            parameters->number
        );

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------
void app_main(void)
{
    static TaskParameters taskOneParameters =
    {
        "Sensor",
        100
    };

    static TaskParameters taskTwoParameters =
    {
        "Logger",
        200
    };

    //---------------------------------------------------
    xTaskCreate(
        workerTask,
        "SensorTask",
        2048,
        &taskOneParameters,
        1,
        NULL
    );

    //---------------------------------------------------
    xTaskCreate(
        workerTask,
        "LoggerTask",
        2048,
        &taskTwoParameters,
        1,
        NULL
    );
}



