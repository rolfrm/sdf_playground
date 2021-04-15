#include <distance_fields.h>
vec2 p1 = {.x = 0.5, .y = 0.0f}; // center screen
f32 radius = 0.5f / 3.0f;
rgb color1 = {.r = 0.1, .g = 0.1, .b = 0.15};
rgb background = {.r = 1, .g = 1, .b = 1};
f32 width = 0.01;

f32 get_distance(vec2 p){
  return fabs(vec2_len(vec2_sub(p, p1)) - radius) - pixel_size * 0.25;
}

rgb get_color(vec2 p){
  f32 d = get_distance(p);
  return df_mix(color1, background, d);
}
