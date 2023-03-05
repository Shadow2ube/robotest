// /**
//  * @brief Everything in this file is controlled by the main thread unless specified
//  */

#include "main.h"
#include "pros/adi.hpp"
#include "pros/imu.hpp"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include "pros/optical.hpp"
#include "util.h"

#include <atomic>
#include <cwchar>

bool run_intake = true;

util::pose v_current_pos = {0, 0, 0};
util::pose v_goto_pos = {0, 0, 0};

pros::Controller controller(pros::E_CONTROLLER_MASTER);
pros::Controller controller2(pros::E_CONTROLLER_PARTNER);

// X DRIVE
pros::Motor m_fl(11, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor m_fr(20, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor m_bl(12, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor m_br(19, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_COUNTS);

// other functions
pros::Motor m_flywheel(2, pros::E_MOTOR_GEARSET_36, true, pros::E_MOTOR_ENCODER_ROTATIONS);
pros::Motor m_suckL(1, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_ROTATIONS); // brings disks into magazine
pros::Motor m_suckR(10, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_ROTATIONS); // brings disks into magazine
pros::Motor_Group m_suck({m_suckL, m_suckR});
pros::Motor m_feed(18, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_ROTATIONS);

pros::ADIEncoder s_perp('G', 'H', false);
pros::ADIEncoder s_para('E', 'F', false);

pros::IMU s_imu(13);
pros::Optical s_optical(3);

// pneumatics

pros::ADIDigitalOut p_end_right('B');
pros::ADIDigitalOut p_end_left('C');
pros::ADIDigitalOut p_end_main('D');