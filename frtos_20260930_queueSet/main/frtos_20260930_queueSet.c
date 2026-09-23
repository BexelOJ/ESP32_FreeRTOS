#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

//---------------------------------------------------

#define QUEUE_LENGTH       5
#define QUEUE_ITEM_SIZE    sizeof(int)

#define TASK_PRIORITY      5
#define TASK_STACK_SIZE    4096

//---------------------------------------------------

static QueueHandle_t queueA;
static QueueHandle_t queueB;
static QueueHandle_t queueC;

static QueueSetHandle_t queueSet;

//---------------------------------------------------

static void producerATask(void *pvParameters)
{
    int value = 100;

    while (1)
    {
        xQueueSend(queueA, &value, portMAX_DELAY);

        printf("Producer A sent: %d\n", value);

        value++;

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------

static void producerBTask(void *pvParameters)
{
    int value = 200;

    while (1)
    {
        xQueueSend(queueB, &value, portMAX_DELAY);

        printf("Producer B sent: %d\n", value);

        value++;

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

//---------------------------------------------------

static void producerCTask(void *pvParameters)
{
    int value = 300;

    while (1)
    {
        xQueueSend(queueC, &value, portMAX_DELAY);

        printf("Producer C sent: %d\n", value);

        value++;

        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}

//---------------------------------------------------

static void consumerTask(void *pvParameters)
{
    QueueSetMemberHandle_t selectedQueue;

    int value;

    while (1)
    {
        selectedQueue = xQueueSelectFromSet(
            queueSet,
            portMAX_DELAY
        );

        if (selectedQueue == queueA)
        {
            if (xQueueReceive(queueA, &value, 0) == pdPASS)
            {
                printf(
                    "Consumer received from Queue A: %d\n",
                    value
                );
            }
        }
        else if (selectedQueue == queueB)
        {
            if (xQueueReceive(queueB, &value, 0) == pdPASS)
            {
                printf(
                    "Consumer received from Queue B: %d\n",
                    value
                );
            }
        }
        else if (selectedQueue == queueC)
        {
            if (xQueueReceive(queueC, &value, 0) == pdPASS)
            {
                printf(
                    "Consumer received from Queue C: %d\n",
                    value
                );
            }
        }
    }
}

//---------------------------------------------------

void app_main(void)
{
    queueA = xQueueCreate(
        QUEUE_LENGTH,
        QUEUE_ITEM_SIZE
    );

    queueB = xQueueCreate(
        QUEUE_LENGTH,
        QUEUE_ITEM_SIZE
    );

    queueC = xQueueCreate(
        QUEUE_LENGTH,
        QUEUE_ITEM_SIZE
    );

    if (queueA == NULL ||
        queueB == NULL ||
        queueC == NULL)
    {
        printf("Queue creation failed\n");
        return;
    }

    //---------------------------------------------------

    queueSet = xQueueCreateSet(
        QUEUE_LENGTH * 3
    );

    if (queueSet == NULL)
    {
        printf("Queue set creation failed\n");
        return;
    }

    //---------------------------------------------------

    xQueueAddToSet(queueA, queueSet);
    xQueueAddToSet(queueB, queueSet);
    xQueueAddToSet(queueC, queueSet);

    //---------------------------------------------------

    xTaskCreate(
        producerATask,
        "ProducerA",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        NULL
    );

    xTaskCreate(
        producerBTask,
        "ProducerB",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        NULL
    );

    xTaskCreate(
        producerCTask,
        "ProducerC",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        NULL
    );

    xTaskCreate(
        consumerTask,
        "Consumer",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        NULL
    );

    printf("Queue Set demonstration started\n");

    //---------------------------------------------------
}



