#include <distance_fields.h>
vec2 p1 = {.x = -0.5, .y = 0.4}; // top left coordinate.
vec2 p2 = {.x = 0.9, .y = 0.9}; // bottom right coordinate..
rgb color0 = RGB(0.6,0.9, 0.8); // light blue
rgb color1 = RGB(0.6,0.7, 0.8); // light blue
rgb color2 = RGB(1.0,1.0,1.0); // white
rgb color3 = RGB(1.0,0.6,0.6); // white
rgb black = RGB(0,0,0);
rgb grayish = {.r = 0.1, .g = 0.1, .b = 0.1};
rgb white = RGB(1,1,1);
//rgb background = RGB(1,1,1);//black;//{.r = 1, .g = 1, .b = 1};
rgb background = RGB(0,0,0);
vec2 center1 = {.x = 0.4, .y = .1};
vec2 center2 = {.x = 0.55, .y = .4};
vec2 center3 = {.x = 0.5, .y = 9};

f32 radius1 = 0.25f / 3.0f;
f32 radius2 = 0.5f / 3.0f;
f32 radius3 = 8.3f;

f32 get_distance(vec2 p, vec2 center, f32 radius){
  return vec2_len(vec2_sub(p, center)) - radius;
}

f32 get_distance_outline(vec2 p, vec2 center, f32 radius){
  return fabs(vec2_len(vec2_sub(p, center)) - radius) - pixel_size * 0.25;
}

rgb gradient(vec2 p, rgb color1, rgb color2, vec2 start, vec2 stop){
  vec2 lvec = vec2_sub(stop, start);
  f32 ratio = vec2_dot(vec2_sub(p, start), lvec) / vec2_dot(lvec, lvec);
  ratio = CLAMP(ratio,0,1.0);
  rgb c = rgb_blend(color1, color2, ratio);
  return c;
}

rgb circle(vec2 p, rgb bg, rgb color1, rgb color2, vec2 p1, vec2 p2, vec2 center, f32 radius){
  // (P - P1) dot (P - P2)
  // Linearily interpolate the colors.
  f32 d = get_distance(p, center, radius);
  if(d <= pixel_size * 2){
    rgb c = df_mix(gradient(p, color1, color2, p1, p2), bg, d);
    //f32 d2 = get_distance_outline(p, center, radius);
    //rgb c2 = df_mix(gradient(p, white, grayish, p1, p2), c, d2);
    return c;
  }
  return bg;
}


rgb get_color(vec2 p){
  rgb a = circle(p, background, color0, color2, p1, p2, center1, radius1);
  a = rgb_blend(a, background, 0.8);
  rgb b = circle(p, a, color3, color2, p1, p2, center2, radius2);
  b = rgb_blend(b, background, 0.8);
 
  rgb c = circle(p, b, color1, color2, p1, p2, center3, radius3);
  return c;

  
}
