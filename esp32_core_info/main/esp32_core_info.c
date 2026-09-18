#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_chip_info.h"
#include "esp_system.h"

void app_main(void)
{
    esp_chip_info_t chip_info;

    esp_chip_info(&chip_info);

    printf("-------------------------------------------\n");
    printf("ESP32 CPU Information\n");
    printf("-------------------------------------------\n");

    printf("CPU cores     : %d\n", chip_info.cores);

    printf("CPU frequency : %d MHz\n",
           CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ);

    printf("Chip revision  : %d\n",
           chip_info.revision);

    printf("FreeRTOS cores : %d\n",
           portNUM_PROCESSORS);

    printf("-------------------------------------------\n");
}





