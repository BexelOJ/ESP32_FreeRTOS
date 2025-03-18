#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED1_PIN GPIO_NUM_15  // LED1 connected to GPIO15 (on-board LED for most ESP32 boards)
#define LED2_PIN GPIO_NUM_2 // LED2 connected to GPIO2 (or you can use any other pin)
#define LED3_PIN GPIO_NUM_17 // LED2 connected to GPIO15 (or you can use any other pin)

void led1_task(void *parameter) {
    // Configure LED1_PIN as an output
    gpio_config_t io_conf1;
    io_conf1.intr_type = GPIO_INTR_DISABLE;
    io_conf1.pin_bit_mask = (1ULL << LED1_PIN);
    io_conf1.mode = GPIO_MODE_OUTPUT;
    io_conf1.pull_up_en = GPIO_PULLUP_DISABLE;
    io_conf1.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio_config(&io_conf1);

    while (true) {
        gpio_set_level(LED1_PIN, 1);  // Turn ON LED1
        vTaskDelay(500 / portTICK_PERIOD_MS); // Wait for 500 ms
        gpio_set_level(LED1_PIN, 0);  // Turn OFF LED1
        vTaskDelay(500 / portTICK_PERIOD_MS); // Wait for 500 ms
    }
}

void led2_task(void *parameter) {
    // Configure LED2_PIN as an output
    gpio_config_t io_conf2;
    io_conf2.intr_type = GPIO_INTR_DISABLE;
    io_conf2.pin_bit_mask = (1ULL << LED2_PIN);
    io_conf2.mode = GPIO_MODE_OUTPUT;
    io_conf2.pull_up_en = GPIO_PULLUP_DISABLE;
    io_conf2.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio_config(&io_conf2);

    while (true) {
        gpio_set_level(LED2_PIN, 1);  // Turn ON LED2
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Wait for 1000 ms (1 second)
        gpio_set_level(LED2_PIN, 0);  // Turn OFF LED2
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Wait for 1000 ms (1 second)
    }
}

void led3_task(void *parameter) {
// Configure LED3_PIN as an output
    gpio_config_t io_conf3;
    io_conf3.intr_type = GPIO_INTR_DISABLE;
    io_conf3.pin_bit_mask = (1ULL << LED3_PIN);
    io_conf3.mode = GPIO_MODE_OUTPUT;
    io_conf3.pull_up_en = GPIO_PULLUP_DISABLE;
    io_conf3.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio_config(&io_conf3);

    while (true) {
        gpio_set_level(LED3_PIN, 1);  // Turn ON LED2
        vTaskDelay(2000 / portTICK_PERIOD_MS); // Wait for 1000 ms (1 second)
        gpio_set_level(LED3_PIN, 0);  // Turn OFF LED2
        vTaskDelay(2000 / portTICK_PERIOD_MS); // Wait for 1000 ms (1 second)
    }
}

void app_main() {
    // Create the two tasks
    xTaskCreate(led1_task, "LED1 Task", 2048, NULL, 1, NULL);
    xTaskCreate(led2_task, "LED2 Task", 2048, NULL, 2, NULL);
    xTaskCreate(led3_task, "LED3 Task", 2048, NULL, 3, NULL);
}
