#include "main.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include "util.h"

// extern const float c_lateral_dist; // mm
// extern const float c_forward_offset; //mm
// extern const float c_wheel_OD; //mm
// extern const float c_dist_per_deg; //mm

extern util::pose v_current_pos;

extern pros::Controller controller;

extern pros::Motor m_fl;
extern pros::Motor m_fr;
extern pros::Motor m_bl;
extern pros::Motor m_br;

extern pros::Motor m_flywheel;
extern pros::Motor_Group m_suck;
extern pros::Motor m_feed;
