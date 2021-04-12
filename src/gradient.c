#include <distance_fields.h>
vec2 p1 = {.x = 0, .y = 0}; // top left coordinate.
vec2 p2 = {.x = 1, .y = 1}; // bottom right coordinate..
rgb color1 = {.r = 0.6, .g = 0.6, .b = 1.0}; // light blue
rgb color2 = {.r = 1.0, .g = 1.0, .b = 1.0}; // white
// https://en.wikipedia.org/wiki/Floyd%E2%80%93Steinberg_dithering
dithering = DITHER_FLOYD_STEINBERG; // dither to avoid banding

f32 get_distance(vec2 p){
  UNUSED(p);
  return FLT_NEGATIVE_INFINITY;     // Everything is inside the field.
}

rgb get_color(vec2 p){
  // (P - P1) dot (P - P2)
  // Linearily interpolate the colors.
  f32 ratio = vec2_dot(vec2_sub(p, p1), vec2_sub(p2, p1));
  return rgb_blend(color1, color2, ratio);
}
