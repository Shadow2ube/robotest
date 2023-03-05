#include "main.h"
#include "autons.h"
#include "config.h"
#include "location.h"
#include "pid.h"
#include "pros/misc.hpp"
#include "pros/rtos.h"
#include "pros/rtos.hpp"
#include "pros/screen.hpp"
#include "selection/selection.h"
#include "wrappers.h"
#include <cstdlib>
#include <utility>

#define IEIE(X, x, Y, y, z)                                                    \
  {                                                                            \
    if (X) {                                                                   \
      x;                                                                       \
    } else if (Y) {                                                            \
      y;                                                                       \
    } else {                                                                   \
      z;                                                                       \
    }                                                                          \
  }
#define IE(X, x, y)                                                            \
  {                                                                            \
    if (X) {                                                                   \
      x;                                                                       \
    } else {                                                                   \
      y;                                                                       \
    }                                                                          \
  }

void t_update_flywheel(void *param) {
  flywheel_setpoint = -50;
  while (true) {
    pros::delay(2);
    if (pros::competition::is_disabled())
      continue;
    flywheel_controller.calc(flywheel_setpoint,
                             m_flywheel.get_actual_velocity());
    std::cout << (m_flywheel.get_voltage()) << std::endl;
  }
}

void t_update_colors(void *param) {
  double current_color = -1;
  bool wait = false;

  while (true) {
    pros::delay(2);
    if (pros::competition::is_disabled())
      continue;
    if (s_optical.get_proximity() > 100) {
      s_optical.set_led_pwm(100);
      wait = true;
    } else {
      s_optical.set_led_pwm(0);
      run_intake = true;
    }

    if (s_optical.get_proximity() > 240) {
      if (wait) {
        pros::delay(100);
        wait = false;
      }
      if (current_color == -1)
        current_color = s_optical.get_hue();
      else if (!EPM(s_optical.get_hue(), current_color, 8)) {
        run_intake = false;
      }
    } else {
      s_optical.set_led_pwm(0);
      current_color = -1;
    }
  }
}

void t_update_location(void *param) {
  while (true) {
    find_location();
    pros::delay(2);
  }
}

void initialize() {
  pros::lcd::initialize();
  s_imu.tare();
  s_imu.reset(true);
  Task update_flywheel(t_update_flywheel);
  Task update_colors(t_update_colors);
  Task update_location(t_update_location);
  selector::init();
}

void disabled() {}

void competition_initialize() {}

bool is_idle = true;
bool is_slow_idle = false;

void autonomous() {
  switch (selector::auton) {
  case 0: { // skills
    auton_skills();
    break;
  }
  case 1:
  case -1: { // left
    auton_left();
    break;
  }
  case 2:
  case -2: { // right
    auton_right();
    break;
  }
  }
}

void handle_drive() {
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
}

void opcontrol() {
  if (!pros::competition::is_connected()) {
    //   // run auton here if not in competition
    // autonomous();
  }

  while (true) {
    // find_location();
    handle_drive();

    // drive control
    if (controller.get_digital_new_press(E_CONTROLLER_DIGITAL_X)) {
      is_idle = !is_idle;
    }

    IEIE(controller.get_digital(E_CONTROLLER_DIGITAL_L2) ||
             controller.get_digital(E_CONTROLLER_DIGITAL_R1),
         intake(true), controller.get_digital(E_CONTROLLER_DIGITAL_R2),
         intake(true, 127, true), intake(false));

    if (controller.get_digital(E_CONTROLLER_DIGITAL_L1)) {
      flywheel(-100);
    } else if (is_idle && !is_slow_idle) {
      flywheel(idle);
    } else if (is_idle && is_slow_idle) {
      flywheel(slow_idle);
    } else {
      flywheel(0);
    };

    IE(
        controller.get_digital(E_CONTROLLER_DIGITAL_R1), { m_feed = 110; },
        m_feed = 0;);

    if (controller.get_digital_new_press(E_CONTROLLER_DIGITAL_A)) {
      is_slow_idle = !is_slow_idle;
    }

    if (controller2.get_digital(E_CONTROLLER_DIGITAL_L1) &&
        controller2.get_digital(E_CONTROLLER_DIGITAL_R1) &&
        controller2.get_digital(E_CONTROLLER_DIGITAL_L2) &&
        controller2.get_digital(E_CONTROLLER_DIGITAL_R2)) {
      // region ENGAME_STUFF
      if (controller2.get_digital(E_CONTROLLER_DIGITAL_X))
        p_end_main.set_value(1);

      // if (controller2.get_digital(E_CONTROLLER_DIGITAL_Y))
      //   p_end_left.set_value(1);

      // endregion
    }
    pros::delay(2);
  }
}
