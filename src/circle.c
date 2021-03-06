#include <distance_fields.h>
vec2 p1 = {.x = 0.5, .y = .5}; // center screen
f32 radius = 0.5f / 3.0f;
rgb color1 = {.r = 0.85, .g = 0.85, .b = 0.8};
rgb background = RGB(1,1,1);
f32 get_distance(vec2 p){
  return vec2_len(vec2_sub(p, p1)) - radius;
}


rgb get_color(vec2 p){
  f32 d = get_distance(p);
  return df_mix(color1, background, d);
}
