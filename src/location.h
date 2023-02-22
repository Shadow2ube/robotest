#ifndef LOCATION_H
#define LOCATION_H

#include "main.h"
#include "config.h"

void find_location();
void move_to(float x, float y, float h, int speed=63);
void move_to(util::pose, int speed=63);

#endif