#include <stdio.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_heap_caps.h"

//---------------------------------------------------
static void printHeapInfo(const char *label)
{
    size_t freeHeap = esp_get_free_heap_size();
    size_t minimumFreeHeap = esp_get_minimum_free_heap_size();
    size_t largestBlock =
        heap_caps_get_largest_free_block(MALLOC_CAP_8BIT);

    printf("\n");
    printf("---------------------------------------------------\n");
    printf("%s\n", label);
    printf("Free Heap     : %u bytes\n", (unsigned int)freeHeap);
    printf("Minimum Free  : %u bytes\n", (unsigned int)minimumFreeHeap);
    printf("Largest Block : %u bytes\n", (unsigned int)largestBlock);
    printf("---------------------------------------------------\n");
}

//---------------------------------------------------
static void mallocFreeTask(void *pvParameters)
{
    printHeapInfo("Initial Heap");

    //---------------------------------------------------
    printf("Allocating 1024 bytes...\n");

    void *memory = malloc(1024);

    if (memory == NULL)
    {
        printf("malloc() failed\n");

        vTaskDelete(NULL);
    }

    //---------------------------------------------------
    printHeapInfo("After malloc(1024)");

    //---------------------------------------------------
    printf("Writing data into allocated memory...\n");

    unsigned char *buffer = (unsigned char *)memory;

    for (int i = 0; i < 1024; i++)
    {
        buffer[i] = (unsigned char)(i & 0xFF);
    }

    //---------------------------------------------------
    printf("Data written successfully\n");

    //---------------------------------------------------
    vTaskDelay(pdMS_TO_TICKS(2000));

    //---------------------------------------------------
    printf("Freeing allocated memory...\n");

    free(memory);

    memory = NULL;

    //---------------------------------------------------
    printHeapInfo("After free()");

    //---------------------------------------------------
    vTaskDelay(pdMS_TO_TICKS(2000));

    //---------------------------------------------------
    printf("Allocating 4096 bytes...\n");

    memory = malloc(4096);

    if (memory == NULL)
    {
        printf("malloc() failed\n");
    }
    else
    {
        printf("malloc() succeeded\n");

        printHeapInfo("After malloc(4096)");

        //---------------------------------------------------
        free(memory);

        memory = NULL;

        printf("4096 bytes freed\n");

        printHeapInfo("Final Heap");
    }

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
        mallocFreeTask,
        "MallocFreeTask",
        2048,
        NULL,
        1,
        NULL
    );
}



