#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//---------------------------------------------------

#define TASK_STACK_SIZE    4096
#define TASK_PRIORITY      5

//---------------------------------------------------

typedef enum
{
    STATE_IDLE,
    STATE_RUNNING,
    STATE_ERROR

} SystemState;

//---------------------------------------------------

static const char *stateToString(SystemState state)
{
    switch (state)
    {
        case STATE_IDLE:
            return "IDLE";

        case STATE_RUNNING:
            return "RUNNING";

        case STATE_ERROR:
            return "ERROR";

        default:
            return "UNKNOWN";
    }
}

//---------------------------------------------------

static SystemState nextState(SystemState currentState)
{
    switch (currentState)
    {
        case STATE_IDLE:
            return STATE_RUNNING;

        case STATE_RUNNING:
            return STATE_ERROR;

        case STATE_ERROR:
            return STATE_IDLE;

        default:
            return STATE_IDLE;
    }
}

//---------------------------------------------------

static void stateMachineTask(void *pvParameters)
{
    SystemState currentState = STATE_IDLE;

    while (1)
    {
        printf(
            "[STATE MACHINE][Core %d] Current state = %s\n",
            xPortGetCoreID(),
            stateToString(currentState)
        );

        vTaskDelay(pdMS_TO_TICKS(2000));

        SystemState newState = nextState(currentState);

        printf(
            "[STATE MACHINE] Transition: %s -> %s\n",
            stateToString(currentState),
            stateToString(newState)
        );

        currentState = newState;
    }
}

//---------------------------------------------------

void app_main(void)
{
    xTaskCreatePinnedToCore(
        stateMachineTask,
        "StateMachineTask",
        TASK_STACK_SIZE,
        NULL,
        TASK_PRIORITY,
        NULL,
        0
    );

    printf("State machine demonstration started\n");

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//---------------------------------------------------



