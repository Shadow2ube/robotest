#include "config.h"
#include "pros/motors.h"

#ifndef WRAPPERS_H
#define WRAPPERS_H

inline void forward(int t, int s = 127) {
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

inline void backward(int t, int s = 127) {
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

inline void left(int t, int s = 127) {
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

inline void right(int t, int s = 127) {
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

inline void turn(int t, bool right = false, int s = 127) {
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

inline void intake(bool run, int speed = 127, bool bwd = false) {
  if (run_intake) {
    m_suck = run ? (bwd ? -speed : speed) : 0;
  } else {
    m_suck = 0;
    // run_intake = true;
  }
}

inline void brake(bool on,
                  pros::motor_brake_mode_e brake_mode = E_MOTOR_BRAKE_HOLD) {
  // auto brake_mode = E_MOTOR_BRAKE_BRAKE;
  auto coast_mode = E_MOTOR_BRAKE_COAST;
  if (on) {
    m_fr.set_brake_mode(brake_mode);
    m_br.set_brake_mode(brake_mode);
    m_fl.set_brake_mode(brake_mode);
    m_bl.set_brake_mode(brake_mode);
  } else {
    m_fr.set_brake_mode(coast_mode);
    m_br.set_brake_mode(coast_mode);
    m_fl.set_brake_mode(coast_mode);
    m_bl.set_brake_mode(coast_mode);
  }
}

inline void wait_until_speed(int speed, int pm = 5) {
  speed *= speed > 0 ? -1 : 1;
  while (NPM<int>(m_flywheel.get_actual_velocity(), speed, pm)) {
    pros::delay(2);
  }
}

/**
 * @brief Shoots X number of discs in a row
 * Shoots a number of discs in a row
 * Blocks thread until complete
 *
 * @param discs number of discs to shoot
 * @param fw_speed speed of flywheel
 */
inline void volley(int discs, int fw_speed) {
  bool ri = run_intake;
  intake(false);
  for (int i = 0; i < discs; ++i) {
    wait_until_speed(fw_speed);
    intake(true);
    m_feed = 127;
    pros::delay(130); // just shoot 1
    intake(false);
    m_feed = 0;
  }
  intake(ri);
}

inline void flywheel(int speed = -100) {
  speed *= speed > 0 ? -1 : 1;
  flywheel_setpoint = speed; // speed * k = voltage
}

#endif