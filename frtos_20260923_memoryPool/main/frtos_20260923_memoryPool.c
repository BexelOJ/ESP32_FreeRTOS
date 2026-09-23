#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

//---------------------------------------------------

#define POOL_BLOCK_COUNT   5
#define POOL_BLOCK_SIZE    64

//---------------------------------------------------

typedef struct
{
    int id;
    char message[POOL_BLOCK_SIZE - sizeof(int)];
} MemoryBlock;

//---------------------------------------------------

static MemoryBlock memoryPool[POOL_BLOCK_COUNT];

static QueueHandle_t freeBlockQueue;

//---------------------------------------------------

static void memoryPoolInit(void)
{
    for (int i = 0; i < POOL_BLOCK_COUNT; i++)
    {
        MemoryBlock *block = &memoryPool[i];

        xQueueSend(
            freeBlockQueue,
            &block,
            portMAX_DELAY
        );
    }
}

//---------------------------------------------------

static MemoryBlock *memoryPoolAllocate(void)
{
    MemoryBlock *block = NULL;

    if (xQueueReceive(
            freeBlockQueue,
            &block,
            0) == pdPASS)
    {
        return block;
    }

    return NULL;
}

//---------------------------------------------------

static void memoryPoolFree(MemoryBlock *block)
{
    if (block != NULL)
    {
        xQueueSend(
            freeBlockQueue,
            &block,
            portMAX_DELAY
        );
    }
}

//---------------------------------------------------

static void producerTask(void *pvParameters)
{
    while (1)
    {
        MemoryBlock *block = memoryPoolAllocate();

        if (block != NULL)
        {
            block->id = 100;

            snprintf(
                block->message,
                sizeof(block->message),
                "Memory pool block allocated"
            );

            printf(
                "Allocated block: id=%d, address=%p, message=%s\n",
                block->id,
                (void *)block,
                block->message
            );

            vTaskDelay(pdMS_TO_TICKS(2000));

            memoryPoolFree(block);

            printf(
                "Freed block: address=%p\n",
                (void *)block
            );
        }
        else
        {
            printf("Memory pool is full\n");
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------

void app_main(void)
{
    freeBlockQueue = xQueueCreate(
        POOL_BLOCK_COUNT,
        sizeof(MemoryBlock *)
    );

    if (freeBlockQueue == NULL)
    {
        printf("Failed to create free block queue\n");
        return;
    }

    memoryPoolInit();

    printf("Memory Pool initialized\n");
    printf("Blocks      : %d\n", POOL_BLOCK_COUNT);
    printf("Block size  : %d bytes\n", sizeof(MemoryBlock));
    printf("Total pool  : %d bytes\n",
           POOL_BLOCK_COUNT * sizeof(MemoryBlock));

    xTaskCreate(
        producerTask,
        "ProducerTask",
        4096,
        NULL,
        1,
        NULL
    );
}

//---------------------------------------------------



