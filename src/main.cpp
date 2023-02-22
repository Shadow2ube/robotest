#include "main.h"
#include "config.h"
#include "location.h"
#include "pros/misc.hpp"
#include "pros/rtos.h"
#include "pros/rtos.hpp"
#include "pros/screen.hpp"
#include "wrappers.h"
#include <cstdlib>

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

void t_update_colors(void *param) {
  double current_color = -1;
  bool wait = false;

  while (true) {
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
    pros::delay(2);
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
  Task update_colors(t_update_colors);
  Task update_location(t_update_location);

  pros::screen::set_pen(0xFF00FF);
  pros::screen::draw_rect(1, 1, 479, 239);
}

void disabled() {}

void competition_initialize() {}

int idle = -127;
// int idle = -105;
bool is_idle = true;

// void autonomous() {
//   brake(true);

//   m_flywheel = -127;
//   intake(true);
//   forward(180);
//   right(820);
//   pros::delay(200);
//   backward(320, 100);
//   pros::delay(100);
//   pros::delay(150);
//   intake(false);
//   // pros::delay(90);

//   forward(150, 63);
//   turn(30, true);
//   pros::delay(2000);
//   // shoot();
//   intake(true);
//   m_feed = 40;
//   pros::delay(1000);
//   intake(false);
//   m_feed = 0;
//   pros::delay(500);
//   intake(true);
//   m_feed = 40;
//   pros::delay(1500);
//   intake(false);
//   m_feed = 0;

//   intake(false);
//   brake(false);
// }

// void autonomous() {
//   auto prev_idle = idle;
//   brake(true);

//   m_flywheel = -127;
//   pros::delay(1000);
//   intake(true);
//   forward(1200, 70);
//   pros::delay(500);
//   turn(130, true, 127);
//   backward(600, 70);
//   // pros::delay(1400);
//   pros::delay(100); //REMOVE
//   // m_feed = 60;
//   // pros::delay(1000);
//   // m_feed = 0;

//   forward(600, 70);
//   turn(365);
//   pros::delay(100);
//   forward(900);
//   pros::delay(100);
//   turn(365, true);
//   //shoot again
//   // turn(200);
//   // backward(1000);
//   // turn(100, true);
//   // backward(350);

//    pros::delay(100);
//   brake(false);
//   idle = prev_idle;
// }

void autonomous() {
  m_fl = -63;
  m_fr = 63;
  m_bl = 63;
  m_br = -63;
  pros::delay(1000);
  move_to(0, -20, 0);
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

// void handle_drive() {
//   m_fr = -controller.get_analog(E_CONTROLLER_ANALOG_RIGHT_X) -
//          controller.get_analog(E_CONTROLLER_ANALOG_LEFT_X);
//   m_br = -controller.get_analog(E_CONTROLLER_ANALOG_RIGHT_X) +
//          controller.get_analog(E_CONTROLLER_ANALOG_LEFT_X);
//   m_fl = controller.get_analog(E_CONTROLLER_ANALOG_RIGHT_X) +
//          controller.get_analog(E_CONTROLLER_ANALOG_LEFT_X);
//   m_bl = controller.get_analog(E_CONTROLLER_ANALOG_RIGHT_X) -
//          controller.get_analog(E_CONTROLLER_ANALOG_LEFT_X);
// }

void opcontrol() {
  if (!pros::competition::is_connected()) {
    //   // run auton here if not in competition
    // autonomous();
  }
  move_to(0, 0, util::pi/2);
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
         intake(true, true), intake(false));

    IEIE(controller.get_digital(E_CONTROLLER_DIGITAL_L1), m_flywheel = -127,
         is_idle, m_flywheel = idle, m_flywheel = 0);

    IE(
        controller.get_digital(E_CONTROLLER_DIGITAL_R1), { m_feed = 110; },
        m_feed = 0;);

    if (controller2.get_digital(E_CONTROLLER_DIGITAL_L1) &&
        controller2.get_digital(E_CONTROLLER_DIGITAL_R1) &&
        controller2.get_digital(E_CONTROLLER_DIGITAL_L2) &&
        controller2.get_digital(E_CONTROLLER_DIGITAL_R2)) {
      // region ENGAME_STUFF
      if (controller2.get_digital(E_CONTROLLER_DIGITAL_X))
        p_end_main.set_value(1);

      if (controller2.get_digital(E_CONTROLLER_DIGITAL_Y))
        p_end_left.set_value(1);

      if (controller2.get_digital(E_CONTROLLER_DIGITAL_A))
        p_end_right.set_value(1);

      // endregion
    }

    pros::delay(2);
  }
}
