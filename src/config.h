#include "main.h"
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include "pros/optical.hpp"
#include "util.h"

#ifndef CONFIG_H
#define CONFIG_H

/**
 * @brief Equal to, plus or minus
 *
 * @tparam T - the type of variable
 * @param X - the value to be within the range
 * @param Y - the value X must be near
 * @param y - the range
 * @return true - (Y + y > X > Y - y)
 * @return false - !(Y + y > X > Y - y)
 */
template <typename T> inline bool EPM(T X, T Y, T y) {
  return (X < Y + y) && (X > Y - y);
}

/**
 * @brief Not equal to, plus or minus
 *
 * @tparam T - the type of variable
 * @param X - the value to not be in the range
 * @param Y - the value X must not be near
 * @param y - the range
 * @return true - (Y + y < X < Y - y)
 * @return false - !(Y + y < X) or !(X < Y - y)
 */
template <typename T> inline bool NPM(T X, T Y, T y) {
  return (X > Y + y) || (X < Y - y);
}

extern int idle;
extern const int slow_idle;
extern const int run_intake;
extern float flywheel_setpoint;

extern util::pose v_current_pos;
extern util::pose v_goto_pos;

extern pros::Controller controller;
extern pros::Controller controller2;

extern pros::Motor m_fl;
extern pros::Motor m_fr;
extern pros::Motor m_bl;
extern pros::Motor m_br;

extern pros::Motor m_flywheel;
extern pros::Motor_Group m_suck;
extern pros::Motor m_feed;

extern pros::ADIEncoder s_para;
extern pros::ADIEncoder s_perp;

extern pros::IMU s_imu;
extern pros::Optical s_optical;

extern pros::ADIDigitalOut p_end_right;
extern pros::ADIDigitalOut p_end_left;
extern pros::ADIDigitalOut p_end_main;

#endif