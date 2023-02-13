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

  pose operator-(pose b) {
    return {
      x - b.x,
      y - b.y, 
      angle - b.angle
    };
  }
};

struct ordered_pair {
  float x, y;

  ordered_pair operator/(float in) {
    return {x / in, y / in};
  }

  ordered_pair operator*(float in) {
    return {x * in, y * in};
  }

  ordered_pair operator+(ordered_pair b) {
    return {
      x + b.x,
      y + b.y
    };
  }

  ordered_pair operator+(float b) {
    return {
      x + b,
      y + b
    };
  }
};

inline ordered_pair split_vec(float magnitude, float angle) {
  return {
    magnitude * ((float) cos(angle)),
    magnitude * ((float) sin(angle))
  };
}

}

inline float operator ""_deg(long double in) {
  return in * util::pi / 180;
}

#endif // UTIL_H_