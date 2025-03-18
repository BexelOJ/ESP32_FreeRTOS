#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// I2C configuration
#define I2C_MASTER_SCL_IO 22   // SCL GPIO pin
#define I2C_MASTER_SDA_IO 21   // SDA GPIO pin
#define I2C_MASTER_NUM I2C_NUM_0 // I2C port number
#define I2C_MASTER_FREQ_HZ 100000 // I2C clock frequency
#define I2C_MASTER_TX_BUF_DISABLE 0  // Disable tx buffer
#define I2C_MASTER_RX_BUF_DISABLE 0  // Disable rx buffer
#define I2C_MASTER_TIMEOUT_MS 1000 // Timeout for I2C operations

#define TM1637_ADDR 0x70   // I2C address for TM1637 (can vary depending on your module)
#define TM1637_CMD_SET 0x40
#define TM1637_CMD_NOOP 0x00

// Function prototypes
esp_err_t i2c_master_init(void);
esp_err_t i2c_master_write_to_device(uint8_t device_address, uint8_t* data, size_t data_length);
void display_number(int number);

void app_main(void)
{
    // Initialize I2C master
    esp_err_t ret = i2c_master_init();
    if (ret != ESP_OK) {
        ESP_LOGE("I2C", "I2C init failed: %s", esp_err_to_name(ret));
        return;
    }

    // Display numbers on the 7-segment display
    while (1) {
        display_number(1234); // Display 1234 on the 7-segment display
        vTaskDelay(1000 / portTICK_PERIOD_MS);  // Update every second
    }
}

// Initialize I2C master
esp_err_t i2c_master_init(void) {
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    // Initialize I2C
    esp_err_t err = i2c_param_config(I2C_MASTER_NUM, &conf);
    if (err != ESP_OK) {
        return err;
    }

    // Install the I2C driver
    return i2c_driver_install(I2C_MASTER_NUM, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

// Write data to I2C device
esp_err_t i2c_master_write_to_device(uint8_t device_address, uint8_t* data, size_t data_length) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (device_address << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write(cmd, data, data_length, true);
    i2c_master_stop(cmd);

    esp_err_t err = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    return err;
}

// Display a number on the 7-segment display
void display_number(int number) {
    uint8_t data[5];

    // TM1637 command to start data writing
    data[0] = TM1637_CMD_SET;

    // Send the command to the display
    esp_err_t err = i2c_master_write_to_device(TM1637_ADDR, data, sizeof(data), I2C_MASTER_TIMEOUT_MS);
    if (err != ESP_OK) {
        ESP_LOGE("I2C", "Failed to send command: %s", esp_err_to_name(err));
        return;
    }

    // Split the number into its individual digits and send them to the display
    data[0] = 0xC0; // Start at the first display position
    data[1] = (number / 1000) % 10;  // Thousands place
    data[2] = (number / 100) % 10;   // Hundreds place
    data[3] = (number / 10) % 10;    // Tens place
    data[4] = number % 10;           // Ones place

    // Write the digits to the 7-segment display
    err = i2c_master_write_to_device(TM1637_ADDR, data, sizeof(data), I2C_MASTER_TIMEOUT_MS);
    if (err != ESP_OK) {
        ESP_LOGE("I2C", "Failed to send data: %s", esp_err_to_name(err));
    }
}
