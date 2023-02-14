#include "main.h"
#include "config.h"
#include "location.h"
#include "pros/rtos.h"
#include "pros/rtos.hpp"
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
      else if (!EPM(s_optical.get_hue(), current_color, 10)) {
        run_intake = false;
      }
    } else {
      s_optical.set_led_pwm(0);
      current_color = -1;
    }
    pros::delay(2);
  }
}

void initialize() {
  pros::lcd::initialize();
  Task update_colors(t_update_colors);
}

void disabled() {}

int prev_s_encoder_val = 0;
int prev_f_encoder_val = 0;
void getPos() {
  int delta_s = s_encoder.get_value() - prev_s_encoder_val;
  int delta_f = s_encoder.get_value() - prev_f_encoder_val;
}

void competition_initialize() {}

int idle = -127;
// int idle = -105;
bool is_idle = true;

void autonomous() {
  brake(true);

  m_flywheel = -127;
  forward(160);
  right(750);
  pros::delay(100);
  backward(320, 100);
  intake(true);
  pros::delay(150);
  intake(false);
  // pros::delay(90);

  forward(100, 63);
  turn(40, true);
  pros::delay(2000);
  // shoot();
  intake(true);
  m_feed = 40;
  pros::delay(1000);
  intake(false);
  m_feed = 0;
  pros::delay(500);
  intake(true);
  m_feed = 40;
  pros::delay(1500);
  intake(false);
  m_feed = 0;

  intake(false);
  brake(false);
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

  double current_color = s_optical.get_hue();
  intake(true);

  while (true) {
    find_location();
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
      // ENDGAME SHENANIGANS
      if (controller2.get_digital(E_CONTROLLER_DIGITAL_X))
        p_end_main.set_value(E_CONTROLLER_DIGITAL_X);
    }

    pros::delay(2);
  }
}
