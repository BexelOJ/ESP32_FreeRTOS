#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//---------------------------------------------------
#define TASK_STACK_SIZE 2048

//---------------------------------------------------
static void stackOverflowTask(void *pvParameters)
{
    printf("Stack Overflow Task: Started\n");

    //---------------------------------------------------
    char buffer[4096];

    //---------------------------------------------------
    memset(buffer, 'A', sizeof(buffer));

    //---------------------------------------------------
    printf("Stack Overflow Task: Buffer written\n");

    //---------------------------------------------------
    printf("Stack Overflow Task: Waiting...\n");

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------
void app_main(void)
{
    //---------------------------------------------------
    printf("Creating Stack Overflow Task\n");

    //---------------------------------------------------
    xTaskCreate(
        stackOverflowTask,
        "StackOverflowTask",
        TASK_STACK_SIZE,
        NULL,
        1,
        NULL
    );

    //---------------------------------------------------
    while (1)
    {
        printf("app_main: Running\n");

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}



