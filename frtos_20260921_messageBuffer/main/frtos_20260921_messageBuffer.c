#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/message_buffer.h"

//---------------------------------------------------
#define MESSAGE_BUFFER_SIZE 128
#define TRIGGER_LEVEL       1

//---------------------------------------------------
static MessageBufferHandle_t messageBuffer = NULL;

//---------------------------------------------------
static void senderTask(void *pvParameters)
{
    const char *messages[] =
    {
        "Hello from Sender",
        "FreeRTOS Message Buffer",
        "This is a variable length message",
        "Message received successfully"
    };

    int messageIndex = 0;

    while (1)
    {
        const char *message = messages[messageIndex];

        size_t bytesSent = xMessageBufferSend(
            messageBuffer,
            message,
            strlen(message) + 1,
            portMAX_DELAY
        );

        if (bytesSent > 0)
        {
            printf("Sender: Message sent\n");
            printf("Sender: %s\n", message);
        }
        else
        {
            printf("Sender: Failed to send message\n");
        }

        messageIndex++;

        if (messageIndex >= 4)
        {
            messageIndex = 0;
        }

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

//---------------------------------------------------
static void receiverTask(void *pvParameters)
{
    char receiveBuffer[64];

    while (1)
    {
        size_t bytesReceived = xMessageBufferReceive(
            messageBuffer,
            receiveBuffer,
            sizeof(receiveBuffer),
            portMAX_DELAY
        );

        if (bytesReceived > 0)
        {
            printf("Receiver: Message received\n");
            printf("Receiver: %s\n", receiveBuffer);
        }
        else
        {
            printf("Receiver: Failed to receive message\n");
        }
    }
}

//---------------------------------------------------
void app_main(void)
{
    messageBuffer = xMessageBufferCreate(
        MESSAGE_BUFFER_SIZE
    );

    if (messageBuffer == NULL)
    {
        printf("Failed to create message buffer\n");
        return;
    }

    printf("Message buffer created\n");
    printf("Buffer size: %d bytes\n", MESSAGE_BUFFER_SIZE);

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



