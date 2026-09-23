#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/spi_master.h"
#include "esp_err.h"

//---------------------------------------------------

#define SPI_HOST        SPI2_HOST

#define SPI_MOSI_GPIO   23
#define SPI_MISO_GPIO   19
#define SPI_SCLK_GPIO   18
#define SPI_CS_GPIO     5

#define SPI_FREQUENCY   1000000

#define TASK_PRIORITY   5
#define TASK_STACK_SIZE 4096

//---------------------------------------------------

static spi_device_handle_t spiDevice;

//---------------------------------------------------

static void spiTransfer(void)
{
    uint8_t txData[] = "Hello SPI";
    uint8_t rxData[sizeof(txData)] = {0};

    spi_transaction_t transaction =
    {
        .length = sizeof(txData) * 8,
        .tx_buffer = txData,
        .rx_buffer = rxData
    };

    esp_err_t result = spi_device_transmit(
        spiDevice,
        &transaction
    );

    if (result == ESP_OK)
    {
        printf("SPI TX: %s\n", txData);
        printf("SPI RX: %s\n", rxData);
    }
    else
    {
        printf(
            "SPI transfer failed: %s\n",
            esp_err_to_name(result)
        );
    }
}

//---------------------------------------------------

static void spiTask(void *pvParameters)
{
    while (1)
    {
        printf(
            "\nSPI Task running on Core %d\n",
            xPortGetCoreID()
        );

        spiTransfer();

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

//---------------------------------------------------

void app_main(void)
{
    spi_bus_config_t busConfig =
    {
        .mosi_io_num = SPI_MOSI_GPIO,
        .miso_io_num = SPI_MISO_GPIO,
        .sclk_io_num = SPI_SCLK_GPIO,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 128
    };

    ESP_ERROR_CHECK(
        spi_bus_initialize(
            SPI_HOST,
            &busConfig,
            SPI_DMA_CH_AUTO
        )
    );

    spi_device_interface_config_t deviceConfig =
    {
        .clock_speed_hz = SPI_FREQUENCY,
        .mode = 0,
        .spics_io_num = SPI_CS_GPIO,
        .queue_size = 1
    };

    ESP_ERROR_CHECK(
        spi_bus_add_device(
            SPI_HOST,
            &deviceConfig,
            &spiDevice
        )
    );

    xTaskCreate(
        spiTask,
        "SPITask",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        NULL
    );

    printf("SPI task demonstration started\n");

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------



