#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_timer.h"

//---------------------------------------------------

#define TASK_PRIORITY      5
#define TASK_STACK_SIZE    4096

//---------------------------------------------------

static void cpuTaskA(void *pvParameters)
{
    while (1)
    {
        for (volatile int i = 0; i < 500000; i++)
        {
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

//---------------------------------------------------

static void cpuTaskB(void *pvParameters)
{
    while (1)
    {
        for (volatile int i = 0; i < 1000000; i++)
        {
        }

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

//---------------------------------------------------

static void statsTask(void *pvParameters)
{
    char buffer[2048];

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(5000));

        printf("\n");
        printf("============================================\n");
        printf("FreeRTOS Runtime Statistics\n");
        printf("============================================\n");

        vTaskGetRunTimeStats(buffer);

        printf(
            "Task Name          CPU Time       CPU %%\n"
        );

        printf(
            "--------------------------------------------\n"
        );

        printf("%s\n", buffer);

        printf(
            "============================================\n"
        );
    }
}

//---------------------------------------------------

void app_main(void)
{
    printf("Runtime statistics demonstration started\n");

    xTaskCreate(
        cpuTaskA,
        "CPU_Task_A",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        NULL
    );

    xTaskCreate(
        cpuTaskB,
        "CPU_Task_B",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        NULL
    );

    xTaskCreate(
        statsTask,
        "StatsTask",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        NULL
    );
}

//---------------------------------------------------



