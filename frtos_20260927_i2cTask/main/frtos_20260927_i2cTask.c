#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/i2c_master.h"
#include "esp_err.h"

//---------------------------------------------------

#define I2C_SDA_GPIO       21
#define I2C_SCL_GPIO       22

#define I2C_FREQUENCY      100000

#define TASK_PRIORITY      5
#define TASK_STACK_SIZE    4096

//---------------------------------------------------

static i2c_master_bus_handle_t i2cBus;

//---------------------------------------------------

static void i2cScan(void)
{
    printf("\n");
    printf("I2C scan started\n");
    printf("---------------------------------------------\n");

    int deviceCount = 0;

    for (uint8_t address = 1; address < 127; address++)
    {
        i2c_device_config_t deviceConfig =
        {
            .dev_addr_length = I2C_ADDR_BIT_LEN_7,
            .device_address = address,
            .scl_speed_hz = I2C_FREQUENCY
        };

        i2c_master_dev_handle_t device;

        esp_err_t result = i2c_master_bus_add_device(
            i2cBus,
            &deviceConfig,
            &device
        );

        if (result != ESP_OK)
        {
            continue;
        }

        uint8_t dummyData;

        result = i2c_master_receive(
            device,
            &dummyData,
            1,
            10
        );

        if (result == ESP_OK)
        {
            printf(
                "I2C device found at address 0x%02X\n",
                address
            );

            deviceCount++;
        }

        i2c_master_bus_rm_device(device);
    }

    printf("---------------------------------------------\n");

    printf(
        "I2C scan complete: %d device(s) found\n",
        deviceCount
    );
}

//---------------------------------------------------

static void i2cTask(void *pvParameters)
{
    while (1)
    {
        printf(
            "\nI2C Task running on Core %d\n",
            xPortGetCoreID()
        );

        i2cScan();

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

//---------------------------------------------------

void app_main(void)
{
    i2c_master_bus_config_t busConfig =
    {
        .i2c_port = I2C_NUM_0,
        .sda_io_num = I2C_SDA_GPIO,
        .scl_io_num = I2C_SCL_GPIO,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true
    };

    ESP_ERROR_CHECK(
        i2c_new_master_bus(
            &busConfig,
            &i2cBus
        )
    );

    xTaskCreate(
        i2cTask,
        "I2CTask",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        NULL
    );

    printf("I2C task demonstration started\n");

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------



