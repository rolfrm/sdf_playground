#include <distance_fields.h>
rgb white = RGB(1,1,1);
rgb black = RGB(0,0,0);

f32 project(vec2 p, vec2 a, vec2 b){
  vec2 p1 = vec2_sub(p, a);
  vec2 ab = vec2_sub(b, a);
  return vec2_dot(p1, ab) / dot(ab, ab); 
}

f32 line_dist(vec2 p, vec2 a, vec2 b){
  vec2 p1 = vec2_sub(p, a);
  vec2 ab = vec2_sub(b, a);
  f32 r = project(p, a, b);
  if(r < 0.0) return vec2_len(p1);
  if(r > 1.0) return vec2_len(vec2_sub(p, b));
  return vec2_len(vec2_sub(p, vec2_add(a, vec2_scale(ab, r))));
}


rgb pseudo_color(f32 x){
  static rgb col1 = RGB(1,0.6,0.5);
  static rgb col2 = RGB(0.5,0.6,1.0);
  x = sin(x / pixel_size *0.8f) * 0.9 + 0.7;
  return rgb_blend(col1, col2, x);
}

rgb get_color(vec2 p){
  f32 d4 = line_dist(p, vec2_new(0.1,0.5), vec2_new(0.9, 0.5)) - 0.02;
  f32 d5 = line_dist(p, vec2_new(0.1,0.7), vec2_new(0.9, 0.7)) - 0.02;
  f32 d6 = line_dist(p, vec2_new(0.1,0.3), vec2_new(0.9, 0.3)) - 0.02;
  f32 d7 = line_dist(p, vec2_new(0.5,0.3), vec2_new(0.5, 0.7)) - 0.02;

  f32 d = MIN3(d5,d4, d6);
  d = MIN(d7, d);
  rgb b = pseudo_color(d);
  return df_mix(black, b, d);
}
