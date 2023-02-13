#include "location.h"
#include "config.h"

float prev[3] = {0.0, 0.0, 0.0};

// const float c_lateral_dist = 424; // mm
const float c_lateral_dist = 218; // mm
// const float c_forward_offset = 212; // mm
const float c_forward_offset = 106; // mm
const float c_wheel_circ = 150.8651697118;
const float c_dist_per_deg = c_wheel_circ/900.0; // mm/deg

void find_location() {
  // get the deltas
  float dxf = (m_fl.get_position() * c_dist_per_deg) - prev[0];
  float dxs = (m_br.get_position() * c_dist_per_deg) - prev[1];
  float dxh = s_imu.get_rotation()- prev[2];

  // update the values
  prev[0] = dxf;
  prev[1] = dxs;
  prev[2] = dxh;

  // calculate the important things
  float phi = (dxf - dxs) / c_lateral_dist; // rotation angle
  float delta_middle_pos = ((dxf + dxs) / 2) * 1.9024323957059384; // displacement
  float delta_perp_pos = (dxh - (c_forward_offset * phi)) * 1.958695232580583; // perp displacement

  auto heading = v_current_pos.angle;

  float delta_x = delta_middle_pos * cos(heading) - delta_perp_pos * sin(heading);
  float delta_y = delta_middle_pos * sin(heading) - delta_perp_pos * cos(heading);

  v_current_pos.x = delta_x;
  v_current_pos.y = delta_y;
  v_current_pos.angle = -(phi- util::pi/4);

  // std::cout << "\033[2J\033[1;1H"
  //   << "x: " << v_current_pos.x << "\n"
  //   << "y: " << v_current_pos.y << "\n"
  //   << "h: " << v_current_pos.angle << "\n"
  //   << std::endl;
  // printf("\033[2J\033[1;1H");
  // std::cout.flush();
  // printf("x: %f\ny: %f\nh: %f", v_current_pos.x, v_current_pos.y, v_current_pos.angle);

}

/**
 * @brief Get the deltas of each wheel to get to a desired pose
 * 
 * @param to pose - pose to go to
 */
util::ordered_pair get_wheel_deltas(util::pose to) {
  auto [delta_x, delta_y, phi] = to - v_current_pos;
  auto dx = util::split_vec(delta_x, phi);
  auto dy = util::split_vec(delta_y, phi);
  auto rotation = phi * c_forward_offset;
  
  auto distance = dx + dy;

  std::cout << "\033[2J\033[1;1H"
    << "phi: " << phi << "\n"
    << "dist x: " << distance.x << "\n"
    << "dist y: " << distance.y << "\n"
    << std::endl;

  return distance / c_wheel_circ * 900.0 / 2;
}

#define EPM(X, Y, y) ((X < Y + y) && (X > Y - y))

void move_to(util::pose to, int speed) {
  auto d = get_wheel_deltas(to);

  std::vector<double> p {
    m_fr.get_position(),
    m_fl.get_position(),
    m_br.get_position(),
    // m_bl.get_position(),
  };

  m_fr.move_relative(d.y, speed);
  m_fl.move_relative(d.x, speed);
  m_br.move_relative(d.x, speed);
  m_bl.move_relative(d.y, speed);
  // m_bl encoder is broken

  while (
    !EPM(m_fr.get_position() - p[0], d.y, 5) ||
    !EPM(m_fl.get_position() - p[1], d.x, 5) ||
    !EPM(m_br.get_position() - p[2], d.x, 5) ||
    !EPM(m_bl.get_position() - p[0], d.y, 5)
  ) {
    pros::delay(2);
  }

  // std::cout // << "\033[2J\033[1;1H"
  //   << "fl: " << m_fl.get_position() << "\n"
  //   << "fr: " << m_fr.get_position() << "\n"
  //   << "bl: " << m_bl.get_position() << "\n"
  //   << "br: " << m_br.get_position() << "\n"
  //   << std::endl;
}