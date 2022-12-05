#include "main.h"
#include "config.h"
#include "location.h"
#include "pros/rtos.hpp"

void initialize() {
  pros::lcd::initialize();
  s_imu.reset(true);
  s_imu.tare();
}

void disabled() {}

void competition_initialize() {}

void forward(int t, int s = 127) {
  m_fl = s;
  m_fr = s;
  m_bl = s;
  m_br = s;
  pros::delay(t);

  m_fl = 0;
  m_bl = 0;
  m_fr = 0;
  m_br = 0;
}

void backward(int t, int s = 127) {
  m_fl = -s;
  m_fr = -s;
  m_bl = -s;
  m_br = -s;
  pros::delay(t);

  m_fl = 0;
  m_bl = 0;
  m_fr = 0;
  m_br = 0;
}

void left(int t, int s = 127) {
  m_fl = -s;
  m_fr = s;
  m_bl = s;
  m_br = -s;
  pros::delay(t);

  m_fl = 0;
  m_bl = 0;
  m_fr = 0;
  m_br = 0;
}

void right(int t, int s = 127) {
  m_fl = s;
  m_fr = -s;
  m_bl = -s;
  m_br = s;
  pros::delay(t);

  m_fl = 0;
  m_bl = 0;
  m_fr = 0;
  m_br = 0;
}

void turn(int t, bool right = false, int s = 127) {
  if (right) {

    m_fl = s;
    m_bl = s;
    m_fr = -s;
    m_br = -s;
  } else {
    m_fl = -s;
    m_bl = -s;
    m_fr = s;
    m_br = s;
  }

  pros::delay(t);

  m_fl = 0;
  m_bl = 0;
  m_fr = 0;
  m_br = 0;
}

int idle = -63;
bool is_idle = true;

void flywheel(bool on) {
  if (on) {
    m_flywheel2 = -127;
    m_flywheel = -127;
  } else if (is_idle) {
    m_flywheel = idle;
    m_flywheel2 = idle;
  }
}

void autonomous() {
  flywheel(false);
  // bacward(1500);
  turn(250);
  pros::delay(2500);
  p_feed.set_value(true);
  pros::delay(100);
  p_feed.set_value(false);
  pros::delay(1000);
  p_feed.set_value(true);
  pros::delay(100);
  p_feed.set_value(false);
  flywheel(false);
}

auto median(double a, double b, double c) {
  if ((a <= b) && (b <= c))
    return b; // a b c
  if ((a <= c) && (c <= b))
    return c; // a c b
  if ((b <= a) && (a <= c))
    return a; // b a c
  if ((b <= c) && (c <= a))
    return c; // b c a
  if ((c <= a) && (a <= b))
    return a; // c a b
  return b;   // c b a
}

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

  v_velocities += {accel_global_x, accel_global_y};
  // auto [a_gx, a_gy] = util::average(v_velocities, {1.618, 1.147, 0.835,
  // 0.658});
  printf("%f, %f, %f\n", v_velocities[0].x, v_velocities[1].x,
         v_velocities[2].x);
  auto a_gx = median(v_velocities[0].x, v_velocities[1].x, v_velocities[2].x);
  auto a_gy = median(v_velocities[0].y, v_velocities[1].y, v_velocities[2].y);

  return {heading, a_gx, a_gy, 0.0};
}

void opcontrol() {
  if (!pros::competition::is_connected()) {
    // run auton only if not in competition
    autonomous();
  }
  while (true) {
    // auto imu = handle_imu(s_imu);
    // pros::lcd::print(0, "%f, %f, %f", imu.x, imu.y, imu.z);
    // pros::lcd::print(1, "%f", imu.heading);
    // printf("%f, %f\n", imu.x, imu.y);
    // printf("\x1B[2J\x1B[H");

    // location::update_position(v_position, imu.x, imu.y, 0.002, v_velocity,
    // imu.heading);

    // drive control
    m_fr = controller.get_analog(E_CONTROLLER_ANALOG_LEFT_Y) -
           controller.get_analog(E_CONTROLLER_ANALOG_RIGHT_X) -
           controller.get_analog(E_CONTROLLER_ANALOG_LEFT_X);
    m_br = controller.get_analog(E_CONTROLLER_ANALOG_LEFT_Y) -
           controller.get_analog(E_CONTROLLER_ANALOG_RIGHT_X) +
           controller.get_analog(E_CONTROLLER_ANALOG_LEFT_X);
    m_fl = controller.get_analog(E_CONTROLLER_ANALOG_LEFT_Y) +
           controller.get_analog(E_CONTROLLER_ANALOG_RIGHT_X) +
           controller.get_analog(E_CONTROLLER_ANALOG_LEFT_X);
    m_bl = controller.get_analog(E_CONTROLLER_ANALOG_LEFT_Y) +
           controller.get_analog(E_CONTROLLER_ANALOG_RIGHT_X) -
           controller.get_analog(E_CONTROLLER_ANALOG_LEFT_X);

    if (controller.get_digital(E_CONTROLLER_DIGITAL_R1))
      m_suck = 127;
    else if (controller.get_digital(E_CONTROLLER_DIGITAL_L1))
      m_suck = -127;
    else
      m_suck = 0;

    if (controller.get_digital(E_CONTROLLER_DIGITAL_R2)) {
      m_flywheel = 127;
      m_flywheel2 = 127;
    } else if (controller.get_digital(E_CONTROLLER_DIGITAL_L2)) {
      m_flywheel = -127;
      m_flywheel2 = -127;
    } else if (is_idle) {
      m_flywheel = idle;
      m_flywheel2 = idle;
    } else {
      m_flywheel = 0;
      m_flywheel2 = 0;
    }

    if (controller.get_digital_new_press(E_CONTROLLER_DIGITAL_X)) {
      is_idle = !is_idle;
    }

    p_feed.set_value(controller.get_digital(E_CONTROLLER_DIGITAL_A));

    if (controller.get_digital(E_CONTROLLER_DIGITAL_Y) &&
        controller.get_digital(E_CONTROLLER_DIGITAL_Y) &&
        controller.get_digital(E_CONTROLLER_DIGITAL_Y)) {
      p_l.set_value(controller.get_digital(E_CONTROLLER_DIGITAL_Y));
      p_r.set_value(controller.get_digital(E_CONTROLLER_DIGITAL_B));
    }
    // aim
    // if (controller.get_digital(E_CONTROLLER_DIGITAL_L1)) {
    // aim(s_position);
    // }

    pros::delay(2);
  }
}
