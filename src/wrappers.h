#include "config.h"

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
