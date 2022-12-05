#include <cmath>
#include <vector>
#include "main.h"
#include "buffer.h"

#ifndef UTIL_H_
#define UTIL_H_

namespace util {

constexpr double pi = 3.14159265358979323846;

/**
 * @brief A struct to hold onto an X and Y
 *
 */
struct ordered_pair {
  double x, y;

  ordered_pair operator+(ordered_pair b) {
    return {this->x + b.x, this->y + b.y};
  }

  ordered_pair operator-(ordered_pair b) {
    return {this->x - b.x, this->y - b.y};
  }

  ordered_pair operator*(double b) { return {this->x * b, this->y * b}; }

  ordered_pair operator+=(std::pair<double, double> b) {
    // printf("%f + %f, %f + %f\n", this-> x, x, this->y, y);
    return {this->x + std::get<0>(b), this->y + std::get<1>(b)};
  }

  template<typename T>
  ordered_pair operator/(T b) {
    return {this->x/b, this->y/b};
  }
};


/**
 * @brief Takes a vector and it's angle and splits it into x and y components
 *
 * @param m magnitude of vector
 * @param a angle of vector
 * @return ordered_pair x, y values of vector
 */
inline auto split_vec(double m, double a) -> util::ordered_pair {
  return {m * cosf(a),m * sinf(a)};
}

template<std::size_t S>
inline auto average(Buffer<ordered_pair, S> values, std::array<double, S> bias) -> ordered_pair {
  ordered_pair average = {0.0, 0.0};
  for (int i = 0; i < S; ++i) {
    average.x += values[i].x * bias[i];
    average.y += values[i].y * bias[i];
  }
  return average / S;
}

}

#endif // UTIL_H_