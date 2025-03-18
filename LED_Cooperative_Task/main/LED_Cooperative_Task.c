#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

// Define the GPIO pins for LEDs
#define LED1_PIN GPIO_NUM_2  // LED1 connected to GPIO2 (on-board LED for most ESP32 boards)
#define LED2_PIN GPIO_NUM_15 // LED2 connected to GPIO15
#define LED3_PIN GPIO_NUM_17 // LED3 connected to GPIO17

// Task for LED1 blinking
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
        vTaskDelay(500 / portTICK_PERIOD_MS); // Wait for 1 second
        gpio_set_level(LED1_PIN, 0);  // Turn OFF LED1
        vTaskDelay(500 / portTICK_PERIOD_MS); // Wait for 1 second
        
        // Optionally yield here to allow the scheduler to switch (if needed)
        taskYIELD();  // Yield control back to the scheduler
    }
}

// Task for LED2 blinking
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
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Wait for 1 second
        gpio_set_level(LED2_PIN, 0);  // Turn OFF LED2
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Wait for 1 second
        
        // Optionally yield here to allow the scheduler to switch (if needed)
        taskYIELD();  // Yield control back to the scheduler
    }
}

// Task for LED3 blinking
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
        gpio_set_level(LED3_PIN, 1);  // Turn ON LED3
        vTaskDelay(2000 / portTICK_PERIOD_MS); // Wait for 1 second
        gpio_set_level(LED3_PIN, 0);  // Turn OFF LED3
        vTaskDelay(2000 / portTICK_PERIOD_MS); // Wait for 1 second
        
        // Optionally yield here to allow the scheduler to switch (if needed)
        taskYIELD();  // Yield control back to the scheduler
    }
}

// The main function that creates tasks (entry point)
void app_main() {
    // Create three tasks: led1_task, led2_task, led3_task
    xTaskCreate(led1_task, "LED1 Task", 2048, NULL, 1, NULL);
    xTaskCreate(led2_task, "LED2 Task", 2048, NULL, 1, NULL);
    xTaskCreate(led3_task, "LED3 Task", 2048, NULL, 1, NULL);
}



