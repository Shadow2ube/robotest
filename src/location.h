// compile_commands.json was at std=c++17, now its std=c++20
#include <cmath>

#include "main.h"
#include "util.h"

#ifndef LOCATION_H_
#define LOCATION_H_

namespace location {

inline auto get_displacement(double v1, double t, double a) -> double {
  return v1 * t + (a / 2) * (t * t);
}

inline void update_position(util::ordered_pair &position, double a_gx, double a_gy,
                            double time, util::ordered_pair &velocity,
                            double heading) {

  printf("POSITION: x: %f, y: %f\n", position.x, position.y);
  printf("ACCEL (M/S): x: %f, y: %f\n", a_gx, a_gy);
  printf("VEOLOCITY: x: %f, y: %f\n", velocity.x, velocity.y);
  printf("HEADING: x: %f\n", heading);
  printf("POSITION: x: %f, y: %f\n", position.x, position.y);
  printf("\x1B[2J\x1B[H");


  auto &[v1_gx, v1_gy] = velocity;

  // auto [a_lxx, a_lxy] = split_vec(a_lx, heading);:w

  // auto [a_lyx, a_lyy] = split_vec(a_lx, heading - util::pi / 2);
  // double a_gx = a_lxx + a_lxy;
  // double a_gy = a_lyx + a_lyy;

  double d_gx = get_displacement(v1_gx, time, a_gx);
  double d_gy = get_displacement(v1_gy, time, a_gy);

  v1_gx = a_gx * time + v1_gx;
  v1_gy = a_gy * time + v1_gy;

  position.x += d_gx;
  position.y += d_gy;

  // printf("\x1B[2J\x1B[H%f\n", d_gy);
  // printf("%f, %f\n", d_gx, d_gy);
  position += {d_gx, d_gy};
  // printf("%f, %f\n", position.x, position.y);
}

} // namespace location

#endif // LOCATION_H_