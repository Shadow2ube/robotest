/**
 * @brief Everything in this file is controlled by the main thread unless specified
 */

#include "main.h"
#include "pros/adi.hpp"
#include "pros/imu.hpp"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include "util.h"

#include <atomic>
#include <cwchar>

util::pose v_current_pos = {0, 0, -util::pi/4};

pros::Controller controller(pros::E_CONTROLLER_MASTER);
pros::Controller controller2(pros::E_CONTROLLER_PARTNER);

// X DRIVE
pros::Motor m_fl(11, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor m_fr(20, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor m_bl(12, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor m_br(19, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_COUNTS);

// other functions
pros::Motor m_flywheel(2, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_ROTATIONS);
pros::Motor m_suckL(1, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_ROTATIONS); // brings disks into magazine
pros::Motor m_suckR(10, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_ROTATIONS); // brings disks into magazine
pros::Motor_Group m_suck({m_suckL, m_suckR});
pros::Motor m_feed(18, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_ROTATIONS);

pros::ADIEncoder s_encoder('H', 'G', false);
pros::ADIEncoder f_encoder('E', 'F', false);

pros::IMU s_imu(13);

// pneumatics

pros::ADIDigitalOut p_end_main('D');