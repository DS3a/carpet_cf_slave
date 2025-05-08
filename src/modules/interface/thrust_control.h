#pragma once

typedef struct {
    double thrust_m1;
    double thrust_m2;
    double thrust_m3;
    double thrust_m4;
} __attribute__((packed)) motors_thrust_t;

void thrustControlTaskInit();
bool thrustControlTaskTest();

