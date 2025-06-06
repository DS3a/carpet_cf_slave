#include "config.h"
#include "debug.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "static_mem.h"
#include "task.h"
#include "app_channel.h"

#include "thrust_ctrl.h"

static void thrustListenerTask(void*);
STATIC_MEM_TASK_ALLOC(thrustListenerTask, THRUST_LISTENER_TASK_STACKSIZE);

static bool isThrustListenerTaskInit = false;

void thrustListenerTaskInit() {
  STATIC_MEM_TASK_CREATE(thrustListenerTask, thrustListenerTask, THRUST_LISTENER_TASK_NAME, NULL, THRUST_LISTENER_TASK_PRI);
  isThrustListenerTaskInit = true;
}

bool thrustListenerTaskTest() {
  return isThrustListenerTaskInit;
}

static void thrustListenerTask(void *parameters) {
  motors_thrust_t thrusts = {0.0, 0.0, 0.0, 0.0};
  // thrusts.thrust_m1 = 50.0;

  // motors_thrust_t rx_packet;
  vTaskDelay(pdMS_TO_TICKS(2000)); // Adjust the delay as needed
  while (1) {
    if (appchannelReceiveDataPacket(&thrusts, sizeof(thrusts), APPCHANNEL_WAIT_FOREVER)) {
      thrustControlTaskEnqueue(thrusts);
    }
    vTaskDelay(pdMS_TO_TICKS(1)); // Adjust the delay as needed
  }
}