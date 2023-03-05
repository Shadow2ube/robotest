#ifndef PID_H
#define PID_H

#include "config.h"
#include "main.h"

struct pid_controller {
  const float kV, kP, kI, kD, precision, iteration_time;
  float prev_error, integral;
  std::function<void(float)> func;

  auto calc(float setpoint, float current) {
    if (setpoint == 0) {
      func(0);
      return;
    }
    auto error = setpoint - current;
    integral = integral + error * iteration_time;
    auto out = (kV * setpoint + kP * error + kI * integral +
                kD * ((error - prev_error) / iteration_time)) *
               precision;

    std::cout << "\033[2J\033[1;1H"
              << "error: " << error
              << "\ncurrent rpm: " << m_flywheel.get_actual_velocity()
              << std::endl;
    prev_error = error;
    func(out);
  }
};

static pid_controller flywheel_controller = {
    // .kP=-0.1,.kD=-1.2, .bias=-100, .iteration_time=0.002,
    .kV = 0.15,
    .kP = 0.05,
    .kI = 0.1,
    .kD = 0.001,
    .precision = 1000,
    .iteration_time = 0.002,
    .prev_error = 0,
    .func = [](float velocity) {
      std::cout << "output: " << velocity << std::endl;
      m_flywheel.move_voltage(velocity);
    }};

/*
1. Get position of robot -> DONE
2. Get
*/

#endif