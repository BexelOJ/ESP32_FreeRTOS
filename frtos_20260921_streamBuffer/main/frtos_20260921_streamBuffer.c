#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/stream_buffer.h"

//---------------------------------------------------
#define STREAM_BUFFER_SIZE       64
#define TRIGGER_LEVEL            1

//---------------------------------------------------
static StreamBufferHandle_t streamBuffer = NULL;

//---------------------------------------------------
static void senderTask(void *pvParameters)
{
    const char *messages[] =
    {
        "Hello from Sender\n",
        "FreeRTOS Stream Buffer\n",
        "Data is flowing...\n"
    };

    int messageIndex = 0;

    while (1)
    {
        const char *message = messages[messageIndex];

        size_t bytesSent = xStreamBufferSend(
            streamBuffer,
            message,
            strlen(message),
            portMAX_DELAY
        );

        printf(
            "Sender: Sent %d bytes\n",
            (int)bytesSent
        );

        messageIndex++;

        if (messageIndex >= 3)
        {
            messageIndex = 0;
        }

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

//---------------------------------------------------
static void receiverTask(void *pvParameters)
{
    char receiveBuffer[32];

    while (1)
    {
        size_t bytesReceived = xStreamBufferReceive(
            streamBuffer,
            receiveBuffer,
            sizeof(receiveBuffer) - 1,
            portMAX_DELAY
        );

        receiveBuffer[bytesReceived] = '\0';

        printf(
            "Receiver: Received %d bytes\n",
            (int)bytesReceived
        );

        printf(
            "Receiver: %s",
            receiveBuffer
        );
    }
}

//---------------------------------------------------
void app_main(void)
{
    streamBuffer = xStreamBufferCreate(
        STREAM_BUFFER_SIZE,
        TRIGGER_LEVEL
    );

    if (streamBuffer == NULL)
    {
        printf("Failed to create stream buffer\n");
        return;
    }

    printf("Stream buffer created\n");
    printf("Buffer size: %d bytes\n", STREAM_BUFFER_SIZE);

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

    //---------------------------------------------------
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



