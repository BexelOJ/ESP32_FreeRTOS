#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"

// This function will print the task name
void vTaskFunction(void *pvParameters) {
    char *pcTaskName;

    pcTaskName = (char *)pvParameters;
    for (;;) {
        ESP_LOGI("TASK", "%s", pcTaskName);  // Printing the task name using ESP-IDF logging
        vTaskDelay(250 / portTICK_PERIOD_MS); // Delay 250 ms
    }
}

void app_main(void)
{
    // Create a task with name "Task1" and assign it to the vTaskFunction
    xTaskCreate(vTaskFunction, "Task1", 2048, "Task1 is Running", 5, NULL);

    // Create another task with name "Task2" and assign it to the vTaskFunction
    xTaskCreate(vTaskFunction, "Task2", 2048, "Task2 is Running", 5, NULL);
}



// #include <stdio.h>

// void app_main(void)
// {
//   void vTaskFunction(void *pvParameters) {
//     char *pcTaskName;

//     pcTaskName = (char *)pvParameters;
//     for (;;) {
//       vPrintString(pcTaskName);
//       vTaskDelay(250);
//       }
// }      
// }
