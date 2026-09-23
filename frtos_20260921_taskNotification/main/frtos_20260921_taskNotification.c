#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//---------------------------------------------------
static TaskHandle_t receiverTaskHandle = NULL;

//---------------------------------------------------
static void senderTask(void *pvParameters)
{
    int notificationValue = 1;

    while (1)
    {
        printf("Sender: Sending notification\n");

        xTaskNotifyGive(receiverTaskHandle);

        printf("Sender: Notification sent\n");

        notificationValue++;

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

//---------------------------------------------------
static void receiverTask(void *pvParameters)
{
    while (1)
    {
        printf("Receiver: Waiting for notification...\n");

        ulTaskNotifyTake(
            pdTRUE,
            portMAX_DELAY
        );

        printf("Receiver: Notification received!\n");
    }
}

//---------------------------------------------------
void app_main(void)
{
    xTaskCreatePinnedToCore(
        receiverTask,
        "ReceiverTask",
        2048,
        NULL,
        1,
        &receiverTaskHandle,
        1
    );

    //---------------------------------------------------
    xTaskCreatePinnedToCore(
        senderTask,
        "SenderTask",
        2048,
        NULL,
        1,
        NULL,
        0
    );
}



