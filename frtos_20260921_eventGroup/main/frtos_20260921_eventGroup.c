#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

//---------------------------------------------------
#define EVENT_SENSOR_READY    (1 << 0)
#define EVENT_WIFI_CONNECTED  (1 << 1)
#define EVENT_DATA_READY      (1 << 2)

//---------------------------------------------------
static EventGroupHandle_t eventGroup = NULL;

//---------------------------------------------------
static void sensorTask(void *pvParameters)
{
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(2000));

        printf("Sensor Task: Sensor is ready\n");

        xEventGroupSetBits(
            eventGroup,
            EVENT_SENSOR_READY
        );
    }
}

//---------------------------------------------------
static void wifiTask(void *pvParameters)
{
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(3000));

        printf("WiFi Task: WiFi connected\n");

        xEventGroupSetBits(
            eventGroup,
            EVENT_WIFI_CONNECTED
        );
    }
}

//---------------------------------------------------
static void dataTask(void *pvParameters)
{
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(5000));

        printf("Data Task: Data is ready\n");

        xEventGroupSetBits(
            eventGroup,
            EVENT_DATA_READY
        );
    }
}

//---------------------------------------------------
static void monitorTask(void *pvParameters)
{
    EventBits_t events;

    while (1)
    {
        printf("Monitor Task: Waiting for events...\n");

        events = xEventGroupWaitBits(
            eventGroup,

            EVENT_SENSOR_READY |
            EVENT_WIFI_CONNECTED |
            EVENT_DATA_READY,

            pdTRUE,
            pdTRUE,
            portMAX_DELAY
        );

        printf("\nMonitor Task: All events received!\n");

        if (events & EVENT_SENSOR_READY)
        {
            printf("  Sensor ready\n");
        }

        if (events & EVENT_WIFI_CONNECTED)
        {
            printf("  WiFi connected\n");
        }

        if (events & EVENT_DATA_READY)
        {
            printf("  Data ready\n");
        }

        printf("\n");
    }
}

//---------------------------------------------------
void app_main(void)
{
    eventGroup = xEventGroupCreate();

    if (eventGroup == NULL)
    {
        printf("Failed to create event group\n");
        return;
    }

    printf("Event group created\n");

    //---------------------------------------------------
    xTaskCreatePinnedToCore(
        sensorTask,
        "SensorTask",
        2048,
        NULL,
        1,
        NULL,
        0
    );

    //---------------------------------------------------
    xTaskCreatePinnedToCore(
        wifiTask,
        "WiFiTask",
        2048,
        NULL,
        1,
        NULL,
        0
    );

    //---------------------------------------------------
    xTaskCreatePinnedToCore(
        dataTask,
        "DataTask",
        2048,
        NULL,
        1,
        NULL,
        1
    );

    //---------------------------------------------------
    xTaskCreatePinnedToCore(
        monitorTask,
        "MonitorTask",
        2048,
        NULL,
        1,
        NULL,
        1
    );
}



