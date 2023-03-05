#include "main.h"
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

// int idle = -127;
int idle = -90;
int slow_idle = -70;
float flywheel_setpoint = idle;

int curr_volt = 0;
void t_update_flywheel(void *param) {
  // flywheel_voltage = -12000;
  flywheel_setpoint = -50;
  while (true) {
    pros::delay(2);
    if (pros::competition::is_disabled())
      continue;
    flywheel_controller.calc(flywheel_setpoint,
                             m_flywheel.get_actual_velocity());
    // flywheel_controller.calc(-50,
    //  m_flywheel.get_actual_velocity());
    std::cout << (m_flywheel.get_voltage()) << std::endl;
  }
}

void flywheel(int speed = -127) {
  speed *= speed > 0 ? -1 : 1;
  flywheel_setpoint = speed * 0.7874015748031497; // speed * k = voltage
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

  // pros::screen::set_pen(0xFF00FF);
  // pros::screen::draw_rect(1, 1, 479, 239);
  selector::init();
}

void disabled() {}

void competition_initialize() {}

// int idle = -105;
bool is_idle = true;
bool is_slow_idle = false;

// void auton_skills() { // old
//   brake(true);
//   m_flywheel = -90;
//   pros::delay(1000);
//   intake(true);
//   forward(1200, 70);
//   pros::delay(500);
//   // turn(130, true, 127);
//   backward(500);
//   right(800);
//   backward(380);
//   forward(100);
//   turn(335);
//   forward(800);
//   m_feed = 127;
//   // intake(true, 80);
//   pros::delay(2000);
//   turn(175, true);
//   right(200);
//   forward(100);
//   p_end_left.set_value(true);
//   p_end_main.set_value(true);
//   pros::delay(1000);
//   backward(500);
//   brake(false);
// }

// void auton_skills() { // new 1
//   brake(true);
//   m_flywheel = -125;

//   pros::delay(2000);
//   intake(true);
//   backward(180);
//   forward(170);
//   turn(430, true);

// #pragma region volley 1 => preloads
//   intake(true, 100);
//   m_feed = 127;
//   pros::delay(2000);
//   m_feed = 0;
// #pragma endregion

//   turn(100);
//   forward(2600, 80);
//   turn(420, true);
//   pros::delay(1500);

// #pragma region volley 2 => 3 stack
//   m_feed = 127;
//   pros::delay(3000);
//   m_feed = 0;
// #pragma endregion

//   turn(260);
//   // forward(750);
//   // forward(750);
//   // right(150);
//   forward(1700);
//   turn(645, true);

// #pragma region volley 2 => 3 stack
//   m_feed = 127;
//   pros::delay(3000);
//   m_feed = 0;
// #pragma endregion

//   m_flywheel = 0;
//   intake(false);
//   brake(false);
// }

void auton_skills() { // newest
  brake(true);
  flywheel(100);
  // flywheel(0);

  pros::delay(2000); // rev up time
  intake(true);
  backward(180);
  forward(170);
  turn(40);

#pragma region volley 1 => preloads
  intake(true, 100);
  m_feed = 127;
  pros::delay(2000);
  m_feed = 0;
#pragma endregion

  turn(215, true);
  pros::delay(50);
  right(150);
  pros::delay(50);
  intake(false);
  forward(950);
  intake(true);
  // left(150);
  forward(1500, 60);
  pros::delay(500);
  // backward(500);
  turn(380);
  pros::delay(250);

#pragma region volley 2 => 3 stack
  m_feed = 127;
  pros::delay(3000);
  m_feed = 0;
#pragma endregion

  turn(350, true);
  right(80);
  // forward(750);
  // forward(750);
  // right(150);
  forward(2300, 80);
  pros::delay(1000);
  turn(500);

#pragma region volley 3 => 3 row
  m_feed = 127;
  pros::delay(3000);
  m_feed = 0;
#pragma endregion

  turn(140);
  backward(700);
  right(1000, 80);
  left(200, 100);
  forward(300, 100);
  right(600, 100);
  backward(700); // roller

  flywheel(0);
  intake(false);
  brake(false);
}

void auton_right() {
  brake(true);
  m_flywheel = -115;
  pros::delay(1000);
  intake(true);
  forward(1200, 70);
  pros::delay(500);
  turn(160, true);
  // backward(100);
  pros::delay(1500);
  m_feed = 127;
  pros::delay(290);
  intake(false);
  m_feed = 0;
  pros::delay(1500);
  intake(true);
  m_feed = 127;
  pros::delay(310);
  intake(false);
  uint8_t port m_feed = 0;
  pros::delay(1200);
  intake(true);
  m_feed = 127;
  pros::delay(330);
  intake(false);
  m_feed = 0;
  // pros::delay(2000);
  turn(300);
  right(150);
  forward(1000);

  brake(false);
}

void auton_left() {
  brake(true);
  m_flywheel = -100;
  backward(100);
  forward(50);
  turn(350, true);
  intake(true);
  m_feed = 127;
  pros::delay(2000);
  m_flywheel = 0;
  brake(false);
}

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
    // auton_right();
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
      flywheel(-100)
    } else if (is_idle && !is_slow_idle) {
      flywheel(idle)
    } else if (is_idle && is_slow_idle) {
      flywheel(slow_idle);
    }
    else {
      flywheel(0)
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

      if (controller2.get_digital(E_CONTROLLER_DIGITAL_Y))
        p_end_left.set_value(1);

      if (controller2.get_digital(E_CONTROLLER_DIGITAL_A))
        p_end_right.set_value(1);

      // endregion
    }

    if (controller2.get_digital(E_CONTROLLER_DIGITAL_DOWN)) {
      std::cout << "AAA";
      idle = -110;
    } else
      idle = -127;

    pros::delay(2);
  }
}
