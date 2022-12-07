#include "main.h"
#include "wrappers.h"
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

void opcontrol() {
  if (!pros::competition::is_connected()) {
    // run auton here if not in competition
    autonomous();
  }
  while (true) {
    // pros::lcd::print(0, "%f, %f, %f", imu.x, imu.y, imu.z);
    // pros::lcd::print(1, "%f", imu.heading);

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

    if (controller.get_digital(E_CONTROLLER_DIGITAL_DOWN) &&
        controller.get_digital(E_CONTROLLER_DIGITAL_LEFT) &&
        controller.get_digital(E_CONTROLLER_DIGITAL_RIGHT)) {
      p_l.set_value(true);
      p_r.set_value(true);
    }
    pros::delay(2);
  }
}
