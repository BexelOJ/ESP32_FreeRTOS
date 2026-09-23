#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "nvs_flash.h"

//---------------------------------------------------

#define WIFI_SSID       "YOUR_WIFI_SSID"
#define WIFI_PASSWORD   "YOUR_WIFI_PASSWORD"

#define TASK_PRIORITY   5
#define TASK_STACK_SIZE 4096

#define WIFI_CONNECTED_BIT BIT0

//---------------------------------------------------

static const char *TAG = "WiFiTask";

static EventGroupHandle_t wifiEventGroup;

//---------------------------------------------------

static void wifiEventHandler(
    void *arg,
    esp_event_base_t eventBase,
    int32_t eventId,
    void *eventData)
{
    if (eventBase == WIFI_EVENT)
    {
        if (eventId == WIFI_EVENT_STA_START)
        {
            printf("Wi-Fi started\n");

            esp_wifi_connect();
        }
        else if (eventId == WIFI_EVENT_STA_DISCONNECTED)
        {
            printf("Wi-Fi disconnected\n");

            xEventGroupClearBits(
                wifiEventGroup,
                WIFI_CONNECTED_BIT
            );

            esp_wifi_connect();

            printf("Attempting Wi-Fi reconnection...\n");
        }
    }

    if (eventBase == IP_EVENT &&
        eventId == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event =
            (ip_event_got_ip_t *)eventData;

        printf(
            "Wi-Fi connected, IP address: "
            IPSTR "\n",
            IP2STR(&event->ip_info.ip)
        );

        xEventGroupSetBits(
            wifiEventGroup,
            WIFI_CONNECTED_BIT
        );
    }
}

//---------------------------------------------------

static void wifiTask(void *pvParameters)
{
    while (1)
    {
        EventBits_t bits = xEventGroupGetBits(
            wifiEventGroup
        );

        if (bits & WIFI_CONNECTED_BIT)
        {
            printf(
                "Wi-Fi Task: connected, running on Core %d\n",
                xPortGetCoreID()
            );
        }
        else
        {
            printf(
                "Wi-Fi Task: waiting for connection, "
                "running on Core %d\n",
                xPortGetCoreID()
            );
        }

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

//---------------------------------------------------

static void wifiInit(void)
{
    ESP_ERROR_CHECK(
        esp_netif_init()
    );

    ESP_ERROR_CHECK(
        esp_event_loop_create_default()
    );

    esp_netif_create_default_wifi_sta();

    wifi_init_config_t wifiInitConfig =
        WIFI_INIT_CONFIG_DEFAULT();

    ESP_ERROR_CHECK(
        esp_wifi_init(&wifiInitConfig)
    );

    ESP_ERROR_CHECK(
        esp_event_handler_register(
            WIFI_EVENT,
            ESP_EVENT_ANY_ID,
            &wifiEventHandler,
            NULL
        )
    );

    ESP_ERROR_CHECK(
        esp_event_handler_register(
            IP_EVENT,
            IP_EVENT_STA_GOT_IP,
            &wifiEventHandler,
            NULL
        )
    );

    wifi_config_t wifiConfig =
    {
        .sta =
        {
            .ssid = WIFI_SSID,
            .password = WIFI_PASSWORD,

            .threshold.authmode =
                WIFI_AUTH_WPA2_PSK
        }
    };

    ESP_ERROR_CHECK(
        esp_wifi_set_mode(WIFI_MODE_STA)
    );

    ESP_ERROR_CHECK(
        esp_wifi_set_config(
            WIFI_IF_STA,
            &wifiConfig
        )
    );

    ESP_ERROR_CHECK(
        esp_wifi_start()
    );
}

//---------------------------------------------------

void app_main(void)
{
    esp_err_t ret = nvs_flash_init();

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
        ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(
            nvs_flash_erase()
        );

        ret = nvs_flash_init();
    }

    ESP_ERROR_CHECK(ret);

    wifiEventGroup = xEventGroupCreate();

    if (wifiEventGroup == NULL)
    {
        printf("Failed to create Wi-Fi event group\n");
        return;
    }

    wifiInit();

    xTaskCreate(
        wifiTask,
        "WiFiTask",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        NULL
    );

    printf("Wi-Fi task demonstration started\n");

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------



