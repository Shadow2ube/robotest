#ifndef T_HANDLE_ACCEL_H
#define T_HANDLE_ACCEL_H

#include "config.h"
#include "main.h"
#include "util.h"

/**
 * @brief Container for imu data
 *
 */
struct imu_data {
  double heading, x, y, z;
};

/**
 * @brief Takes the imu and returns the heading and global accelerations
 * Z values aren't calculated
 *
 * @param imu The imu to read from
 * @param values
 * @return imu_data
 */
auto handle_imu(pros::Imu imu) -> imu_data;

auto t_handle_imu(void *param) -> void;

#endif // T_HANDLE_ACCEL_H