/** @file   proj2.h
 *  @author Kristian Luptak (xluptak00)
 *  @date   25.4.2025
 *  @brief  simulates processes of cars, trucks, carriage
 */

#ifndef PROJ_H
#define PROJ_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <stdarg.h>


int check_input(char *input, int min, int max);

void car(int car_time, int car_idx, int max_load);

void truck(int truck_time, int truck_idx, int max_load);

void carriage(int carriage_time);

void destroy();

void init();

void write_f(const char *fmt, ...);

void err();

#endif //PROJ_H