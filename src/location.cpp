#include "location.h"
#include "config.h"

float prev[3] = {0.0, 0.0, 0.0};

// const float c_lateral_dist = 424; // mm
const float c_lateral_dist = 218; // mm
// const float c_forward_offset = 212; // mm
const float c_forward_offset = 106; // mm
const float c_dist_per_deg = 259.3385/900.0; // mm/deg

void find_location() {
  // get the deltas
  float dxl = (m_fl.get_position() * c_dist_per_deg) - prev[0];
  float dxr = (m_br.get_position() * c_dist_per_deg) - prev[1];
  float dxh = (m_fr.get_position() * c_dist_per_deg) - prev[2];

  // update the values
  prev[0] = dxl;
  prev[1] = dxr;
  prev[2] = dxh;

  // calculate the important things
  float phi = (dxl - dxr) / c_lateral_dist; // rotation angle
  float delta_middle_pos = ((dxl + dxr) / 2) * 1.9024323957059384; // displacement
  float delta_perp_pos = (dxh - (c_forward_offset * phi)) * 1.958695232580583;

  auto heading = v_current_pos.angle;

  float delta_x = delta_middle_pos * cos(heading) - delta_perp_pos * sin(heading);
  float delta_y = delta_middle_pos * sin(heading) - delta_perp_pos * cos(heading);

  v_current_pos.x = delta_x;
  v_current_pos.y = delta_y;
  v_current_pos.angle = phi;

  // std::cout << "\033[2J\033[1;1H"
  //   << "x: " << v_current_pos.x << "\n"
  //   << "y: " << v_current_pos.y << "\n"
  //   << "h: " << v_current_pos.angle << "\n"
  //   << std::endl;

}