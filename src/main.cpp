#include "main.h"
#include "wrappers.h"
#include "config.h"
#include "pros/rtos.hpp"

inline auto map2(bool A, int a, bool B, int b) -> int {
  /*
   * If A -> a
   * If B -> b
   * If none -> 0
   * If Both -> 0
  */
  return 0;
}

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
  //ASUMING WE START AT THE RIGHT OF OUR NET
  //start roller pointing back
  //turn roller on
  //go right until roller and roller are lined up
  //go back a lil untill roller and roller are touching
  //turn roller off in x seconds after contact is made
  //go left untill robot is beside the net
  //turn robot left till fly wheel faces the net
  //shoot 2 disks while fly wheel is in idle
  //IF EXTRA TIME:
  //line up robot with disks so we can pick up during driver

  right(2000);
  roller(true);
  //2 seconds because roller is on and contact will be made for the remaining time
  backward(2000);
  roller(false);
  forward(1000);
  left(5000);
  turn(1500);
  shoot();
  shoot();
  turn(1500, true);
  right(3000);
  forward(1000);
  flywheel(false);
  // old code

//  right(1400);
//  backward(500);
//  intake(false);
//  pros::delay(100);
//  turn(100, true);

  /*flywheel(false);
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
  flywheel(false);*/
}

bool angle = false;

void opcontrol() {
  // if (!pros::competition::is_connected()) {
  //   // run auton here if not in competition
  //   autonomous();
  // }
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

    if (controller.get_digital(E_CONTROLLER_DIGITAL_R2))
      m_suck = -127;
    else if (controller.get_digital(E_CONTROLLER_DIGITAL_L2))
      m_suck = 127;
    else
      m_suck = 0;

    // if (controller.get_digital(E_CONTROLLER_DIGITAL_R2)) {
    //   m_flywheel = 127;
    //   m_flywheel2 = 127;
    // } else if (controller.get_digital(E_CONTROLLER_DIGITAL_L2)) {
    //   m_flywheel = -127;
    //   m_flywheel2 = -127;
    // } else if (is_idle) {
    //   m_flywheel = idle;
    //   m_flywheel2 = idle;
    // } else {
    //   m_flywheel = 0;
    //   m_flywheel2 = 0;
    // }

    if (controller.get_digital_new_press(E_CONTROLLER_DIGITAL_X)) {
      is_idle = !is_idle;
    }

    if (controller.get_digital_new_press(E_CONTROLLER_DIGITAL_B)) {
      angle = !angle;

      p_l.set_value(angle);
      p_r.set_value(angle);
    }

    if (angle) {}

    // p_feed.set_value(controller.get_digital(E_CONTROLLER_DIGITAL_A));
    m_dpush = controller.get_digital(E_CONTROLLER_DIGITAL_R1);

    /*if (controller.get_digital(E_CONTROLLER_DIGITAL_DOWN) &&
        controller.get_digital(E_CONTROLLER_DIGITAL_LEFT) &&
        controller.get_digital(E_CONTROLLER_DIGITAL_RIGHT)) {
      p_l.set_value(true);
      p_r.set_value(true);
    }*/
    pros::delay(2);
  }
}
