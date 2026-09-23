#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//---------------------------------------------------

#define TASK_STACK_SIZE    4096

#define TASK_PRIORITY      5

//---------------------------------------------------

static TaskHandle_t blockedTaskHandle;

//---------------------------------------------------

static void blockedTask(void *pvParameters)
{
    while (1)
    {
        printf(
            "[TASK] Running on Core %d\n",
            xPortGetCoreID()
        );

        printf("[TASK] Entering BLOCKED state for 3 seconds\n");

        vTaskDelay(pdMS_TO_TICKS(3000));

        printf("[TASK] Delay expired - task is READY again\n");

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

//---------------------------------------------------

static void monitorTask(void *pvParameters)
{
    while (1)
    {
        eTaskState state = eTaskGetState(
            blockedTaskHandle
        );

        printf(
            "[MONITOR] BlockedTask state = %d\n",
            state
        );

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

//---------------------------------------------------

void app_main(void)
{
    xTaskCreatePinnedToCore(
        blockedTask,
        "BlockedTask",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        &blockedTaskHandle,
        0
    );

    xTaskCreatePinnedToCore(
        monitorTask,
        "MonitorTask",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        NULL,
        1
    );

    printf("Blocked task demonstration started\n");

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------



