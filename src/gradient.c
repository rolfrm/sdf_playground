#include <distance_fields.h>
vec2 p1 = {.x = 0, .y = 0}; // top left coordinate.
vec2 p2 = {.x = 0, .y = 1}; // bottom right coordinate..
rgb color1 = {.r = 0.0, .g = 0.0, .b = 0.0}; // light blue
rgb color2 = {.r = 1.0, .g = 1.0, .b = 1.0}; // white

rgb get_color(vec2 p){
  // (P - P1) dot (P - P2)
  // Linearily interpolate the colors.
  f32 n = noise(p);
  vec2 lvec = vec2_sub(p2, p1);
  f32 ratio = vec2_dot(vec2_sub(p, p1), lvec) / vec2_dot(lvec, lvec);
  ratio = CLAMP(ratio,0,1.0);
  return rgb_blend(color1, color2, ratio + 1 * (n - 0.5f) / 128.0f);  
}

