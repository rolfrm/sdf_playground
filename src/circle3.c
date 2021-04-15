#include <distance_fields.h>
vec2 p1 = {.x = 0.1, .y = 0.1}; // top left coordinate.
vec2 p2 = {.x = 0.9, .y = 0.9}; // bottom right coordinate..
rgb color1 = {.r = 0.6, .g = 0.6, .b = 1.0}; // light blue
rgb color2 = {.r = 1.0, .g = 1.0, .b = 1.0}; // white
rgb black = {.r = 0, .g = 0, .b = 0};
rgb grayish = {.r = 0.1, .g = 0.1, .b = 0.1};
rgb white = RGB(1,1,1);
rgb background = {.r = 1, .g = 1, .b = 1};

vec2 center = {.x = 0.7, .y = .7}; // center screen
f32 radius = 0.5f / 3.0f;

f32 get_distance(vec2 p){
  return vec2_len(vec2_sub(p, center)) - radius;
}

f32 get_distance_outline(vec2 p){
  return fabs(vec2_len(vec2_sub(p, center)) - radius) - pixel_size * 0.25;
}

rgb gradient(vec2 p, rgb color1, rgb color2, vec2 start, vec2 stop){
  vec2 lvec = vec2_sub(stop, start);
  f32 ratio = vec2_dot(vec2_sub(p, start), lvec) / vec2_dot(lvec, lvec);
  ratio = CLAMP(ratio,0,1.0);
  rgb c = rgb_blend(color1, color2, ratio);
  return c;
}

rgb get_color(vec2 p){
  // (P - P1) dot (P - P2)
  // Linearily interpolate the colors.
  f32 d = get_distance(p);
  if(d <= pixel_size * 2){
    rgb c = df_mix(gradient(p, color1, color2, p1, p2), background, d);
    f32 d2 = get_distance_outline(p);
    rgb c2 = df_mix(gradient(p, white, black, p1, p2), c, d2);
    return c2;
  }
  return background;
}
