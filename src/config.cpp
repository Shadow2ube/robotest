#include "main.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "util.h"
#include "buffer.h"

util::ordered_pair v_position = {0, 0};
util::ordered_pair v_velocity = {0, 0};
Buffer<util::ordered_pair, 3> v_velocities({0.0, 0.0});

pros::Controller controller(pros::E_CONTROLLER_MASTER);

// X DRIVE
pros::Motor m_fl(2, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor m_fr(9, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor m_bl(1, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor m_br(10, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_COUNTS);

// other functions
pros::Motor m_flywheel(4, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_ROTATIONS);
pros::Motor m_flywheel2(5, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_ROTATIONS);
pros::Motor m_suck(3, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_ROTATIONS); // brings disks into magazine
// pros::Motor m_feed(19, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_DEGREES); // feed disks into flywheel

// pneumatics
pros::ADIDigitalOut p_l('F');
pros::ADIDigitalOut p_feed('G');
pros::ADIDigitalOut p_r('H');

// 1 motor left will figure out what to do with it

pros::Imu s_imu(7);
