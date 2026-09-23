#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//---------------------------------------------------

#define TASK_PRIORITY      5
#define TASK_STACK_SIZE    4096

//---------------------------------------------------

static TaskHandle_t workerATaskHandle = NULL;
static TaskHandle_t workerBTaskHandle = NULL;
static TaskHandle_t workerCTaskHandle = NULL;

//---------------------------------------------------

static void workerATask(void *pvParameters)
{
    while (1)
    {
        ulTaskNotifyTake(
            pdTRUE,
            portMAX_DELAY
        );

        printf(
            "Worker A: processing task on Core %d\n",
            xPortGetCoreID()
        );

        vTaskDelay(pdMS_TO_TICKS(500));

        printf("Worker A: work completed\n");
    }
}

//---------------------------------------------------

static void workerBTask(void *pvParameters)
{
    while (1)
    {
        ulTaskNotifyTake(
            pdTRUE,
            portMAX_DELAY
        );

        printf(
            "Worker B: processing task on Core %d\n",
            xPortGetCoreID()
        );

        vTaskDelay(pdMS_TO_TICKS(700));

        printf("Worker B: work completed\n");
    }
}

//---------------------------------------------------

static void workerCTask(void *pvParameters)
{
    while (1)
    {
        ulTaskNotifyTake(
            pdTRUE,
            portMAX_DELAY
        );

        printf(
            "Worker C: processing task on Core %d\n",
            xPortGetCoreID()
        );

        vTaskDelay(pdMS_TO_TICKS(900));

        printf("Worker C: work completed\n");
    }
}

//---------------------------------------------------

static void coordinatorTask(void *pvParameters)
{
    int job = 0;

    while (1)
    {
        job++;

        printf(
            "\nCoordinator: distributing job %d\n",
            job
        );

        xTaskNotifyGive(workerATaskHandle);

        vTaskDelay(pdMS_TO_TICKS(500));

        xTaskNotifyGive(workerBTaskHandle);

        vTaskDelay(pdMS_TO_TICKS(500));

        xTaskNotifyGive(workerCTaskHandle);

        printf("Coordinator: job %d distributed\n\n", job);

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

//---------------------------------------------------

void app_main(void)
{
    xTaskCreate(
        workerATask,
        "WorkerA",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        &workerATaskHandle
    );

    xTaskCreate(
        workerBTask,
        "WorkerB",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        &workerBTaskHandle
    );

    xTaskCreate(
        workerCTask,
        "WorkerC",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        &workerCTaskHandle
    );

    xTaskCreate(
        coordinatorTask,
        "Coordinator",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        NULL
    );

    printf("Multiple-task coordination demonstration started\n");
}

//---------------------------------------------------



