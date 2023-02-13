#include "main.h"
#include "pros/rtos.h"
#include "wrappers.h"
#include "config.h"
#include "pros/rtos.hpp"
#include "location.h"
#include <cstdlib>

#define IEIE(X, x, Y, y, z) {if (X) {x;} else if (Y) {y;} else {z;}}
#define IE(X, x, y) {if (X) {x;} else {y;}}

void initialize() {
  pros::lcd::initialize();
}

void disabled() {}


int prev_s_encoder_val = 0;
int prev_f_encoder_val = 0;
void getPos(){
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
  intake(true, true);
  pros::delay(150);
  intake(false);
  // pros::delay(90);

  forward(100, 63);
  turn(40, true);
  pros::delay(2000);
  // shoot();
  intake(true, true);
  m_feed = 40;
  pros::delay(1000);
  intake(false);
  m_feed = 0;
  pros::delay(500);
  intake(true, true);
  m_feed = 40;
  pros::delay(1500);
  intake(false);
  m_feed = 0;

  intake(false);
  brake(false);

}

// void autonomous() {
//   brake(true);

//   m_flywheel = -127;
//   intake(true, true);


//   while (f_encoder.get_value() < 1440){
//     forward(150);
//   }



//   right(700);
//   pros::delay(100);
//   intake(true, true);
//   backward(350, 100);
//   pros::delay(200);

//   forward(100, 63);
//   pros::delay(1000);
//   // shoot();
//   intake(true, true);
//   m_feed = 63;
//   pros::delay(1000);
//   intake(false);
//   m_feed = 0;
//   pros::delay(500);
//   intake(true, true);
//   m_feed = 63;
//   pros::delay(1500);
//   intake(false);
//   m_feed = 0;

//   intake(false);
//   brake(false);

// }

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
    autonomous();
  }

  // brake(true); // TODO: remove this for driver
  while (true) {
    find_location();

    // drive control
    if (controller.get_digital_new_press(E_CONTROLLER_DIGITAL_X)) { 
      is_idle = !is_idle;
    }

    handle_drive();

    IEIE (
      controller.get_digital(E_CONTROLLER_DIGITAL_L2) ||
      controller.get_digital(E_CONTROLLER_DIGITAL_R1), m_suck = 127,
      controller.get_digital(E_CONTROLLER_DIGITAL_R2), m_suck = -127,
      m_suck = 0
    );

    IEIE(
      controller.get_digital(E_CONTROLLER_DIGITAL_L1), m_flywheel = -127,
      is_idle, m_flywheel = idle,
      m_flywheel = 0
    );

    IE (
      controller.get_digital(E_CONTROLLER_DIGITAL_R1), {m_feed = 110; },
      m_feed = 0;
    );

    if (
      controller2.get_digital(E_CONTROLLER_DIGITAL_L1)
      && controller2.get_digital(E_CONTROLLER_DIGITAL_R1)
      && controller2.get_digital(E_CONTROLLER_DIGITAL_L2)
      && controller2.get_digital(E_CONTROLLER_DIGITAL_R2)
      ) {
        // ENDGAME SHENANIGANS
        if (controller2.get_digital(E_CONTROLLER_DIGITAL_X))
         p_end_main.set_value(E_CONTROLLER_DIGITAL_X);
      }

    pros::delay(2);
  }
}
