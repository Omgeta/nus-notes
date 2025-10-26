/**
 * CS2106 AY 25/26 Semester 1 - Lab 3
 *
 * Do not modify this file.
 */

#include <semaphore.h>
#include "barrier.h"
#include <stdio.h>

#ifndef H2O_H
#define H2O_H

// ---------------- H2O Coordination ----------------

void h2o_init();
void h2o_destroy();
void oxygen_thread(void (*)(void));
void hydrogen_thread(void (*)(void));

#endif // H2O_H

