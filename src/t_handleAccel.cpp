#include "t_handleAccel.h"

int time = 10;

auto handle_imu(pros::Imu imu) -> imu_data {
  auto accel = imu.get_accel();
  double heading = imu.get_heading() * (util::pi / 180);

  auto accel_local_x = ((floor(accel.x * 100) / 100)) * 9.81;
  auto accel_local_y = ((floor(accel.y * 100) / 100) + 0.02) * 9.81;

  auto [accel_local_xx, accel_local_xy] =
      util::split_vec(accel_local_x, heading);
  auto [accel_local_yx, accel_local_yy] =
      util::split_vec(accel_local_x, heading - (util::pi / 2));

  double accel_global_x = accel_local_xx + accel_local_yx;
  double accel_global_y = accel_local_xy + accel_local_yy;

  // v_velocities += {accel_global_x, accel_global_y};
  // auto [a_gx, a_gy] = util::average(v_velocities, {1.618, 1.147, 0.835,
  // 0.658});

  return {heading, accel_global_x, accel_global_y, 0.0};
}

auto t_handle_imu(void *param) {
  while (true) {
    auto imu = handle_imu(s_imu);
    location::update_position(v_position, imu.x, imu.y, time/1000, v_velocity,
                              imu.heading);
  }
}