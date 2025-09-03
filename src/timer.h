// src/timer.h
#ifndef TIMER_H
#define TIMER_H
#include <stdint.h>
// Initializes the Programmable Interval Timer (PIT)
void pit_init(uint32_t frequency);

#endif // TIMER_H
