#include "main.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "util.h"


extern std::atomic<util::ordered_pair> v_position;
extern std::atomic<util::ordered_pair> v_velocity;

extern pros::Controller controller;

extern pros::Motor m_fl;
extern pros::Motor m_fr;
extern pros::Motor m_bl;
extern pros::Motor m_br;

extern pros::Motor m_flywheel;
extern pros::Motor m_flywheel2;
extern pros::Motor m_suck;
extern pros::Motor m_feed;

extern pros::ADIDigitalOut p_l;
extern pros::ADIDigitalOut p_feed;
extern pros::ADIDigitalOut p_r;

extern pros::Imu s_imu;