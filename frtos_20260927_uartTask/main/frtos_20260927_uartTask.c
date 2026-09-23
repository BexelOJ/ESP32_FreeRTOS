#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/uart.h"

//---------------------------------------------------

#define UART_PORT       UART_NUM_0
#define UART_BAUD_RATE  115200

#define UART_RX_BUFFER  1024
#define UART_TX_BUFFER  1024

#define TASK_PRIORITY   5
#define TASK_STACK_SIZE 4096

//---------------------------------------------------

static void uartTask(void *pvParameters)
{
    uint8_t rxData[UART_RX_BUFFER];

    while (1)
    {
        int length = uart_read_bytes(
            UART_PORT,
            rxData,
            sizeof(rxData) - 1,
            pdMS_TO_TICKS(100)
        );

        if (length > 0)
        {
            rxData[length] = '\0';

            printf(
                "UART Task: received on Core %d: %s",
                xPortGetCoreID(),
                (char *)rxData
            );

            const char *response = "UART Task: data received\r\n";

            uart_write_bytes(
                UART_PORT,
                response,
                strlen(response)
            );
        }
    }
}

//---------------------------------------------------

void app_main(void)
{
    const uart_config_t uartConfig =
    {
        .baud_rate = UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT
    };

    uart_driver_install(
        UART_PORT,
        UART_RX_BUFFER,
        UART_TX_BUFFER,
        0,
        NULL,
        0
    );

    uart_param_config(
        UART_PORT,
        &uartConfig
    );

    xTaskCreate(
        uartTask,
        "UARTTask",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        NULL
    );

    printf("UART task demonstration started\n");

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------



