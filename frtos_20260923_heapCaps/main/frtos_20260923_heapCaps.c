#include <stdio.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_heap_caps.h"

//---------------------------------------------------
static void printHeapInfo(const char *name, uint32_t capabilities)
{
    size_t freeHeap =
        heap_caps_get_free_size(capabilities);

    size_t largestBlock =
        heap_caps_get_largest_free_block(capabilities);

    printf("\n");
    printf("---------------------------------------------------\n");
    printf("%s\n", name);

    printf("Free Heap     : %u bytes\n",
           (unsigned int)freeHeap);

    printf("Largest Block : %u bytes\n",
           (unsigned int)largestBlock);

    printf("---------------------------------------------------\n");
}

//---------------------------------------------------
static void heapCapsTask(void *pvParameters)
{
    //---------------------------------------------------
    printHeapInfo(
        "8-bit Capable Memory",
        MALLOC_CAP_8BIT
    );

    //---------------------------------------------------
    printHeapInfo(
        "Internal Memory",
        MALLOC_CAP_INTERNAL
    );

    //---------------------------------------------------
    printHeapInfo(
        "DMA Capable Memory",
        MALLOC_CAP_DMA
    );

    //---------------------------------------------------
    printf("\nAllocating 1024 bytes with MALLOC_CAP_8BIT\n");

    void *normalMemory =
        heap_caps_malloc(
            1024,
            MALLOC_CAP_8BIT
        );

    if (normalMemory != NULL)
    {
        printf("8-bit allocation successful\n");
    }
    else
    {
        printf("8-bit allocation failed\n");
    }

    //---------------------------------------------------
    printHeapInfo(
        "After 8-bit Allocation",
        MALLOC_CAP_8BIT
    );

    //---------------------------------------------------
    printf("\nAllocating 2048 bytes with MALLOC_CAP_INTERNAL\n");

    void *internalMemory =
        heap_caps_malloc(
            2048,
            MALLOC_CAP_INTERNAL
        );

    if (internalMemory != NULL)
    {
        printf("Internal allocation successful\n");
    }
    else
    {
        printf("Internal allocation failed\n");
    }

    //---------------------------------------------------
    printHeapInfo(
        "After Internal Allocation",
        MALLOC_CAP_INTERNAL
    );

    //---------------------------------------------------
    vTaskDelay(pdMS_TO_TICKS(2000));

    //---------------------------------------------------
    if (normalMemory != NULL)
    {
        heap_caps_free(normalMemory);

        normalMemory = NULL;

        printf("\n8-bit memory freed\n");
    }

    //---------------------------------------------------
    if (internalMemory != NULL)
    {
        heap_caps_free(internalMemory);

        internalMemory = NULL;

        printf("Internal memory freed\n");
    }

    //---------------------------------------------------
    printHeapInfo(
        "After Free",
        MALLOC_CAP_8BIT
    );

    //---------------------------------------------------
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

//---------------------------------------------------
void app_main(void)
{
    //---------------------------------------------------
    xTaskCreate(
        heapCapsTask,
        "HeapCapsTask",
        4096,
        NULL,
        1,
        NULL
    );
}



