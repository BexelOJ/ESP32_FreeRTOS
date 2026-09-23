#include <stdio.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_heap_caps.h"

//---------------------------------------------------
static size_t initialFreeHeap;

//---------------------------------------------------
static void printHeapUsage(void)
{
    size_t freeHeap = esp_get_free_heap_size();

    size_t minimumFreeHeap =
        esp_get_minimum_free_heap_size();

    size_t largestBlock =
        heap_caps_get_largest_free_block(MALLOC_CAP_8BIT);

    size_t usedHeap = initialFreeHeap - freeHeap;

    int usagePercent = 0;

    if (initialFreeHeap > 0)
    {
        usagePercent =
            (int)((usedHeap * 100) / initialFreeHeap);
    }

    //---------------------------------------------------
    printf("\n");
    printf("---------------------------------------------------\n");
    printf("Free Heap       : %u bytes\n",
           (unsigned int)freeHeap);

    printf("Used Heap       : %u bytes\n",
           (unsigned int)usedHeap);

    printf("Heap Usage      : %d %%\n",
           usagePercent);

    printf("Minimum Free    : %u bytes\n",
           (unsigned int)minimumFreeHeap);

    printf("Largest Block   : %u bytes\n",
           (unsigned int)largestBlock);

    printf("---------------------------------------------------\n");
}

//---------------------------------------------------
static void monitorTask(void *pvParameters)
{
    while (1)
    {
        printHeapUsage();

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

//---------------------------------------------------
static void memoryTask(void *pvParameters)
{
    while (1)
    {
        //---------------------------------------------------
        printf("\nMemory Task: Allocating 4096 bytes\n");

        void *memory = malloc(4096);

        if (memory != NULL)
        {
            printf("Memory Task: Allocation successful\n");

            //---------------------------------------------------
            vTaskDelay(pdMS_TO_TICKS(5000));

            //---------------------------------------------------
            free(memory);

            memory = NULL;

            printf("Memory Task: Memory freed\n");
        }
        else
        {
            printf("Memory Task: Allocation failed\n");
        }

        //---------------------------------------------------
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

//---------------------------------------------------
void app_main(void)
{
    //---------------------------------------------------
    initialFreeHeap = esp_get_free_heap_size();

    printf("Initial Free Heap: %u bytes\n",
           (unsigned int)initialFreeHeap);

    //---------------------------------------------------
    xTaskCreate(
        monitorTask,
        "MonitorTask",
        2048,
        NULL,
        2,
        NULL
    );

    //---------------------------------------------------
    xTaskCreate(
        memoryTask,
        "MemoryTask",
        2048,
        NULL,
        1,
        NULL
    );
}



