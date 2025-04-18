#include "main.h"
#include <cmath>
#include <vector>

#ifndef UTIL_H_
#define UTIL_H_

namespace util {

constexpr double pi = 3.14159265358979323846;

struct pose {
  float x;
  float y;
  float angle;

  pose operator-(pose b) { return {x - b.x, y - b.y, angle - b.angle}; }
};
inline float to_rad(float in) { return in * util::pi / 180; }

struct ordered_pair {
  float x, y;

  ordered_pair operator/(float in) { return {x / in, y / in}; }

  ordered_pair operator*(float in) { return {x * in, y * in}; }

  ordered_pair operator+(ordered_pair b) { return {x + b.x, y + b.y}; }

  ordered_pair operator+(float b) { return {x + b, y + b}; }
};

inline ordered_pair split_vec(float magnitude, float angle) {
  return {magnitude * ((float)cos(angle)), magnitude * ((float)sin(angle))};
}

inline std::tuple<float, float> to_vec(float x, float y) {
  return {std::sqrt(x * x + y * y), atan(y / x)};
}

inline float round_down_to(float in, int prec) {
  // TODO: do the things here to round down
  auto d = pow(10, prec);
  auto temp = (int)(in * d + 0.5);
  return (float)(temp / d);
}

inline float abs(float in) {
  if (in >= 0) return in;
  return -in;
}

} // namespace util

inline float operator""_deg(long double in) { return in * util::pi / 180; }

#endif // UTIL_H_