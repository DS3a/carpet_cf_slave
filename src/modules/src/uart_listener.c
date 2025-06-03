#include "config.h"
#include "debug.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "static_mem.h"
#include "task.h"

#include "thrust_ctrl.h"

static void uartListenerTask(void*);
STATIC_MEM_TASK_ALLOC(uartListenerTask, UART_LISTENER_TASK_STACKSIZE);

static bool isUartTaskInit = false;

void uartTaskInit() {
  STATIC_MEM_TASK_CREATE(uartListenerTask, uartListenerTask, UART1_TEST_TASK_NAME, NULL, UART1_TEST_TASK_PRI);
  isUartTaskInit = true;
}

bool uartTaskTest() {
  return isUartTaskInit;
}

static void uartListenerTask(void *parameters) {
  DEBUG_PRINT("UART listener task started.\n");
  
  // Initialize UART here, e.g. uartInit(UART1, UART1_BAUDRATE);
  motors_thrust_t thrusts = {10.9, 10.9, 10.9, 10.9};
  // thrusts.thrust_m1 = 50.0;
  
  vTaskDelay(pdMS_TO_TICKS(2000)); // Adjust the delay as needed
  while (1) {
    // Listen for incoming data on UART
    // For example, you can use uartReceiveData() to read data from UART
    // and process it accordingly.
    
    thrustControlTaskEnqueue(thrusts);
    vTaskDelay(pdMS_TO_TICKS(1)); // Adjust the delay as needed
  }
}