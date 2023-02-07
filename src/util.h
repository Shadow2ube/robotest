#include <cmath>
#include <vector>
#include "main.h"

#ifndef UTIL_H_
#define UTIL_H_

namespace util {

constexpr double pi = 3.14159265358979323846;

struct pose {
  float x;
  float y;
  float angle;
};

}

#endif // UTIL_H_