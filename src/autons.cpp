#include "autons.h"

// void auton_skills() { // old
//   brake(true);
//   m_flywheel = -90;
//   pros::delay(1000);
//   intake(true);
//   forward(1200, 70);
//   pros::delay(500);
//   // turn(130, true, 127);
//   backward(500);
//   right(800);
//   backward(380);
//   forward(100);
//   turn(335);
//   forward(800);
//   m_feed = 127;
//   // intake(true, 80);
//   pros::delay(2000);
//   turn(175, true);
//   right(200);
//   forward(100);
//   p_end_left.set_value(true);
//   p_end_main.set_value(true);
//   pros::delay(1000);
//   backward(500);
//   brake(false);
// }

void auton_skills(bool half) { // newest
  brake(true);
  flywheel(100);
  // flywheel(0);

  pros::delay(2000); // rev up time
  intake(true);
  backward(180);
  forward(170);
  turn(40);

// #pragma region volley 1 => preloads
//   intake(true, 100);
//   m_feed = 127;
//   pros::delay(2000);
//   m_feed = 0;
// #pragma endregion
#pragma region volley 1 => preloads
  volley(2, 80);
#pragma endregion

  turn(215, true);
  pros::delay(50);
  right(150);
  pros::delay(50);
  intake(false);
  forward(950);
  intake(true);
  // left(150);
  forward(1500, 60);
  pros::delay(500);
  // backward(500);
  turn(380);
  pros::delay(250);

// #pragma region volley 2 => 3 stack
//   m_feed = 127;
//   pros::delay(3000);
//   m_feed = 0;
// #pragma endregion
#pragma region volley 2 => 3 stack
  volley(3, 80);
#pragma endregion

  if (half)
    goto skills_end; // skip other parts

  turn(350, true);
  right(80);
  forward(2300, 80);
  pros::delay(1000);
  turn(500);

// #pragma region volley 3 => 3 row
//   m_feed = 127;
//   pros::delay(3000);
//   m_feed = 0;
// #pragma endregion
#pragma region volley 3 => 3 row
  volley(3, 80);
#pragma endregion

  turn(140);
  backward(700);
  right(1000, 80);
  left(200, 100);
  forward(300, 100);
  right(600, 100);
  backward(700); // roller

skills_end: // clean up
  flywheel(0);
  intake(false);
  brake(false);
}

void auton_right() {
  brake(true);
  m_flywheel = -105;
  pros::delay(1000);
  intake(true);
  forward(1200, 70);
  pros::delay(500);
  turn(160, true);
  // backward(100);
  pros::delay(1500);
  m_feed = 127;
  pros::delay(290);
  intake(false);
  m_feed = 0;
  pros::delay(1500);
  intake(true);
  m_feed = 127;
  pros::delay(310);
  intake(false);
  m_feed = 0;
  pros::delay(1200);
  intake(true);
  m_feed = 127;
  pros::delay(330);
  intake(false);
  m_feed = 0;
  // pros::delay(2000);
  turn(300);
  right(150);
  forward(1000);

  brake(false);
}

// void auton_left() {
//   brake(true);
//   m_flywheel = -100;
//   backward(100);
//   forward(50);
//   turn(350, true);
//   intake(true);
//   m_feed = 127;
//   pros::delay(2000);
//   m_flywheel = 0;
//   brake(false);
// }

void auton_left() { auton_skills(true); }