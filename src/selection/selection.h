// #pragma once

#ifndef SELECTION_H
#define SELECTION_H

#include <string>

//selector configuration
#define HUE 360
#define DEFAULT 0
#define AUTONS "Left", "Right", "Do Nothing"

namespace selector{

extern int auton;
static const char *b[] = {AUTONS, ""};
void init(int hue = HUE, int default_auton = DEFAULT, const char **autons = b);

}

#endif
