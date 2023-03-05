#include "location.h"
#include "config.h"
#include "pid.h"
#include <cmath>

float prev[3] = {util::to_rad(s_para.get_value()),
                 util::to_rad(s_perp.get_value()), 0};

// const float c_side_offset = 149;                          // mm
const float c_side_offset = -1719;                          // mm
// const float c_forward_offset = 170;                       // mm
const float c_forward_offset = -1850;                       // mm
const float c_wheel_circ = 219.4402;                      // mm
const float c_dist_per_rad = c_wheel_circ / 2 * util::pi; // mm/rad

pid_controller forward_pid {
  .kP = 1,
  .kD = 1,
  .precision = 0,
  .iteration_time = 0.002,
  .prev_error = 0,
  .func = [](auto value) {
    m_fr = value;
    m_br = value;
    m_fl = value;
    m_bl = value;
  }
};

float calc_phi(float current_angle, float prev_angle) {
  auto diff = current_angle - prev_angle;
  if (diff > util::pi)
    return 2 * util::pi - diff;
  if (diff < -util::pi)
    return 2 * util::pi + diff;
  return diff;
}

void find_location() {
  auto para = util::to_rad(s_para.get_value());
  auto perp = util::to_rad(s_perp.get_value());

  auto delta_left_encoder_pos = (para - prev[0]) * c_dist_per_rad;
  auto delta_center_encoder_pos = (perp - prev[1]) * c_dist_per_rad;
  auto phi = util::round_down_to(
      calc_phi(util::to_rad(s_imu.get_heading()), prev[2]), 3);
  auto delta_middle_pos = delta_left_encoder_pos - (c_side_offset * phi);
  auto delta_perp_pos = delta_center_encoder_pos - (c_forward_offset * phi);

  auto heading = v_current_pos.angle;
  // auto heading = 0;

  auto delta_x =
      delta_middle_pos * cos(heading) - delta_perp_pos * sin(heading);
  auto delta_y =
      delta_middle_pos * sin(heading) + delta_perp_pos * cos(heading);

  v_current_pos.x += delta_x;
  v_current_pos.y += delta_y;
  v_current_pos.angle = util::to_rad(s_imu.get_heading());

  prev[0] = para;
  prev[1] = perp;
  prev[2] = util::to_rad(s_imu.get_heading());

  // std::cout << "\033[2J\033[1;1H"
  //           << "x: " << v_current_pos.x << "\n"
  //           << "y: " << v_current_pos.y << "\n"
  //           << "h: " << heading << "\n"
  //           << std::endl;
}

void move_to(float x, float y, float h, int speed) {
  v_goto_pos = {x, y, h};
  // float g_delta_x = x - v_current_pos.x;
  // float g_delta_y = y - v_current_pos.y;
  // float l_delta_h = h - v_current_pos.angle;

  // auto heading = v_current_pos.angle;
  // float l_delta_x = g_delta_x * cos(heading) - g_delta_y * sin(heading);
  // float l_delta_y = g_delta_x * sin(heading) - g_delta_y * cos(heading);

  // std::cout << "\033[2J\033[1;1H"
  //           << "x: " << l_delta_x << "\n"
  //           << "y: " << l_delta_y << "\n"
  //           << "h: " << heading << "\n"
  //           << std::endl;

}