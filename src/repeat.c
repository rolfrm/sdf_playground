#include <distance_fields.h>
vec2 p1 = {.x = 0.125, .y = .125}; // center screen
rgb color1 = {.r = 0.4, .g = 0.4, .b = 0.5};
rgb color2 = {.r = 1.0, .g = 0.9, .b = 0.9};
rgb background = RGB(1,1,1);
vec2 p2 = {.x = 0.125, .y = .125}; // center screen
vec2 p3 = VEC2(0.1f,0.1f);
vec2 p4 = VEC2(0.9f,0.9f);

// floating point modulus that
// is consisten velow 0.
f32 mod(f32 vec, f32 mod){
  return vec - mod * floor(vec / mod);
}

vec2 repeat(vec2 p, f32 r){
  p.x = mod(p.x, r);
  p.y = mod(p.y, r);
  return p;
}

f32 dist1(vec2 p){
  p = repeat(p, 0.25);
  return vec2_len(vec2_sub(p, p1)) - 0.08;
}

f32 dist2(vec2 p){
  p = vec2_sub(p, p1);
  p = repeat(p, 0.25);
  return vec2_len(vec2_sub(p, p1)) - 0.11;
}

rgb get_color(vec2 p){
  rgb gra = df_gradient(p, color1, color2, p3, p4);
  rgb bg = rgb_blend(color1, color2, 0.5);
  
  f32 d = dist1(p);
  f32 d2 = dist2(p);
  rgb c = df_mix(color1, rgb_blend(bg, gra, 0.5), d);
  c = df_mix(color2, c, d2);
  return c;
}
