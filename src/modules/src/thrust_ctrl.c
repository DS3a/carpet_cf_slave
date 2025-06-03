#include "config.h"
#include "debug.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "static_mem.h"
#include "task.h"

#include "motors.h"
#include "pm.h"

// #include "log.h"

#include "thrust_ctrl.h"

static xQueueHandle motorThrustQueue;
// static float m1_ratiot = 99.0;
STATIC_MEM_QUEUE_ALLOC(motorThrustQueue, 1, sizeof(motors_thrust_t));

static bool isMotorThrustTaskInit = false;

static void thrustControlTask(void *arg);
STATIC_MEM_TASK_ALLOC(thrustControlTask, THRUST_CTRL_TASK_STACKSIZE);

void thrustControlTaskInit() {
    motorThrustQueue = STATIC_MEM_QUEUE_CREATE(motorThrustQueue);

    STATIC_MEM_TASK_CREATE(thrustControlTask, thrustControlTask, THRUST_CTRL_TASK_NAME, NULL, THRUST_CTRL_TASK_PRI);

    isMotorThrustTaskInit = true;
}

bool thrustControlTaskTest() {
    return isMotorThrustTaskInit;
}

static void thrustControlTask(void *arg) {
    // initialize motors_thrust_t with all motors thrust set to 0
    motors_thrust_t thrusts = {0, 0, 0, 0};
    uint16_t ratios[4] = {0, 0, 0, 0};
    while (1) {
        if (pdTRUE == xQueueReceive(motorThrustQueue, &thrusts, portMAX_DELAY)) {
            // check battery voltage
            float current_voltage = pmGetBatteryVoltage();


            for (int id=0; id<4; id++) {
                if (current_voltage >= 2.0f) {
                        ratios[id] = motorsCompensateBatteryVoltage(id, 
                            (&thrusts.thrust_m1)[id] * 65536.0 / 60.0, current_voltage);
                } else {
                    ratios[id] = 0;
                    // call the function that makes the leds red
                }
                // m1_ratiot = ratios[0];
                motorsSetRatio(id, ratios[id]);
            }    
        }
    }
}

void thrustControlTaskEnqueue(motors_thrust_t thrusts) {
    if (motorThrustQueue != NULL) {
        xQueueOverwrite(motorThrustQueue, &thrusts);
    }
}

// LOG_GROUP_START(thrustctrl)

// LOG_ADD(LOG_FLOAT, mone, &m1_ratiot)
// LOG_GROUP_STOP(thrustctrl)