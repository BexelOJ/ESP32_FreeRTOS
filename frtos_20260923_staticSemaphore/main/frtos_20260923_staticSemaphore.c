#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

//---------------------------------------------------
static StaticSemaphore_t semaphoreBuffer;

static SemaphoreHandle_t binarySemaphore;

//---------------------------------------------------
static void senderTask(void *pvParameters)
{
    while (1)
    {
        printf("Sender Task: Sending semaphore\n");

        //---------------------------------------------------
        xSemaphoreGive(binarySemaphore);

        //---------------------------------------------------
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

//---------------------------------------------------
static void receiverTask(void *pvParameters)
{
    while (1)
    {
        //---------------------------------------------------
        if (xSemaphoreTake(
                binarySemaphore,
                portMAX_DELAY) == pdTRUE)
        {
            printf("Receiver Task: Semaphore received\n");
        }
    }
}

//---------------------------------------------------
void app_main(void)
{
    //---------------------------------------------------
    binarySemaphore = xSemaphoreCreateBinaryStatic(
        &semaphoreBuffer
    );

    //---------------------------------------------------
    if (binarySemaphore == NULL)
    {
        printf("Static semaphore creation failed\n");

        return;
    }

    printf("Static semaphore created successfully\n");

    //---------------------------------------------------
    xTaskCreate(
        senderTask,
        "SenderTask",
        2048,
        NULL,
        1,
        NULL
    );

    //---------------------------------------------------
    xTaskCreate(
        receiverTask,
        "ReceiverTask",
        2048,
        NULL,
        1,
        NULL
    );
}



