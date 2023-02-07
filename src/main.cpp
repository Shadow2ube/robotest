#include "main.h"
#include "pros/rtos.h"
#include "wrappers.h"
#include "config.h"
#include "pros/rtos.hpp"
#include "location.h"

#define IEIE(X, x, Y, y, z) {if (X) {x;} else if (Y) {y;} else {z;}}
#define IE(X, x, y) {if (X) {x;} else {y;}}

void initialize() {
  pros::lcd::initialize();
}

void disabled() {}

void competition_initialize() {}

int idle = -105;
bool is_idle = true;

void autonomous() {
  // right of net

  m_flywheel = -127;
  forward(200);
  right(500);
  intake(true, true);
  backward(350);
  pros::delay(200);
  intake(false);

  // brake(true);
  // m_flywheel = idle;
  // forward(200);
  // right(500);
  // turn(50, right);
  // pros::delay(1000);
  // shoot(true);
  // pros::delay(5000);
  // turn(50);
  // backward(350);
  // brake(false);

}

void handle_drive() {
  m_fr = controller.get_analog(E_CONTROLLER_ANALOG_LEFT_Y)
    - controller.get_analog(E_CONTROLLER_ANALOG_RIGHT_X)
    - controller.get_analog(E_CONTROLLER_ANALOG_LEFT_X);
  m_br = controller.get_analog(E_CONTROLLER_ANALOG_LEFT_Y)
    - controller.get_analog(E_CONTROLLER_ANALOG_RIGHT_X)
    + controller.get_analog(E_CONTROLLER_ANALOG_LEFT_X);
  m_fl = controller.get_analog(E_CONTROLLER_ANALOG_LEFT_Y)
    + controller.get_analog(E_CONTROLLER_ANALOG_RIGHT_X)
    + controller.get_analog(E_CONTROLLER_ANALOG_LEFT_X);
  m_bl = controller.get_analog(E_CONTROLLER_ANALOG_LEFT_Y)
    + controller.get_analog(E_CONTROLLER_ANALOG_RIGHT_X)
    - controller.get_analog(E_CONTROLLER_ANALOG_LEFT_X);
}

// void handle_drive() {
//   m_fl = controller.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
//   m_br = controller.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
//   m_bl = -controller.get_analog(E_CONTROLLER_ANALOG_LEFT_X);
//   m_fr = -controller.get_analog(E_CONTROLLER_ANALOG_LEFT_X);
// }

void opcontrol() {
  if (!pros::competition::is_connected()) {
  //   // run auton here if not in competition
    // autonomous();
  }

  brake(true); // TODO: remove this for driver
  while (true) {
    find_location();

    // drive control
    if (controller.get_digital_new_press(E_CONTROLLER_DIGITAL_X)) { 
      is_idle = !is_idle;
    }

    handle_drive();

    IEIE (
      controller.get_digital(E_CONTROLLER_DIGITAL_L2), m_suck = 127,
      controller.get_digital(E_CONTROLLER_DIGITAL_R2), m_suck = -127,
      m_suck = 0
    );

    IEIE(
      controller.get_digital(E_CONTROLLER_DIGITAL_L1), m_flywheel = -127,
      is_idle, m_flywheel = idle,
      m_flywheel = 0
    );

    IE (
      controller.get_digital(E_CONTROLLER_DIGITAL_R1), {m_feed = 110; m_suck = 127;},
      m_feed = 0;
    );

    pros::delay(2);
  }
}
