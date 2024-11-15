#ifndef TIMER_H
#define TIMER_H

#include "driver/timer.h"
#define PUMP_PIN 7
#define LIGHTING_PIN 8

void setup_timer(timer_idx_t timer_num, int seconds);
void updateTimer(int minutes);
#endif // TIMER_H