#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

//---------------------------------------------------
static SemaphoreHandle_t binarySemaphore = NULL;

//---------------------------------------------------
static void senderTask(void *pvParameters)
{
    while (1)
    {
        printf("Sender: Giving binary semaphore\n");

        xSemaphoreGive(binarySemaphore);

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

//---------------------------------------------------
static void receiverTask(void *pvParameters)
{
    while (1)
    {
        printf("Receiver: Waiting for binary semaphore...\n");

        if (xSemaphoreTake(binarySemaphore, portMAX_DELAY) == pdTRUE)
        {
            printf("Receiver: Binary semaphore received!\n");
        }
    }
}

//---------------------------------------------------
void app_main(void)
{
    binarySemaphore = xSemaphoreCreateBinary();

    if (binarySemaphore == NULL)
    {
        printf("Failed to create binary semaphore\n");
        return;
    }

    printf("Binary semaphore created\n");

    xTaskCreatePinnedToCore(
        senderTask,
        "SenderTask",
        2048,
        NULL,
        1,
        NULL,
        0
    );

    xTaskCreatePinnedToCore(
        receiverTask,
        "ReceiverTask",
        2048,
        NULL,
        1,
        NULL,
        1
    );
}

