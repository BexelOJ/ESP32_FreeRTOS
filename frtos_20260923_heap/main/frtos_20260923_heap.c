#include <stdio.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_heap_caps.h"

//---------------------------------------------------
static void printHeapInfo(const char *label)
{
    size_t freeHeap = esp_get_free_heap_size();
    size_t minFreeHeap = esp_get_minimum_free_heap_size();
    size_t largestBlock = heap_caps_get_largest_free_block(MALLOC_CAP_8BIT);

    printf("\n");
    printf("---------------------------------------------------\n");
    printf("%s\n", label);
    printf("Free Heap       : %u bytes\n", (unsigned int)freeHeap);
    printf("Minimum Free    : %u bytes\n", (unsigned int)minFreeHeap);
    printf("Largest Block   : %u bytes\n", (unsigned int)largestBlock);
    printf("---------------------------------------------------\n");
}

//---------------------------------------------------
static void heapTask(void *pvParameters)
{
    printHeapInfo("Initial Heap");

    //---------------------------------------------------
    void *memory = malloc(1024);

    if (memory != NULL)
    {
        printf("Allocated 1024 bytes\n");
    }
    else
    {
        printf("Memory allocation failed\n");
    }

    //---------------------------------------------------
    printHeapInfo("After malloc()");

    //---------------------------------------------------
    vTaskDelay(pdMS_TO_TICKS(2000));

    //---------------------------------------------------
    free(memory);

    printf("Freed 1024 bytes\n");

    //---------------------------------------------------
    printHeapInfo("After free()");

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
        heapTask,
        "HeapTask",
        2048,
        NULL,
        1,
        NULL
    );
}



