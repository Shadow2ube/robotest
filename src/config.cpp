/**
 * @brief Everything in this file is controlled by the main thread unless specified
 */

#include "main.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include "util.h"
#include "buffer.h"

#include <atomic>

std::atomic<util::ordered_pair> v_position({0, 0});
std::atomic<util::ordered_pair> v_velocity({0, 0});

pros::Controller controller(pros::E_CONTROLLER_MASTER);

// X DRIVE
pros::Motor m_fl(3, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor m_fr(8, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor m_bl(2, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor m_br(9, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_COUNTS);

// other functions
pros::Motor m_flywheel(5, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_ROTATIONS);
pros::Motor m_flywheel2(6, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_ROTATIONS);
pros::Motor m_suckL(1, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_ROTATIONS); // brings disks into magazine
pros::Motor m_suckR(10, pros::E_MOTOR_GEARSET_36, true, pros::E_MOTOR_ENCODER_ROTATIONS); // brings disks into magazine
pros::Motor_Group m_suck({m_suckL, m_suckR});
// pros::Motor m_feed(19, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_DEGREES); // feed disks into flywheel

// pneumatics
pros::ADIDigitalOut p_l('G');
pros::ADIDigitalOut p_feed('H');
pros::ADIDigitalOut p_r('H');

// 1 motor left will figure out what to do with it

pros::Imu s_imu(7); // handler: t_handle_imu
