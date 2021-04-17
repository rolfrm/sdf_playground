#include <distance_fields.h>
vec2 p1 = {.x = 0.5, .y = -0.4}; // top left coordinate.
vec2 p2 = {.x = 0.5, .y = 1.5}; // bottom right coordinate..
rgb color0 = RGB(0.6,0.9, 0.8); // light blue
rgb color1 = RGB(0.5,0.6, 0.7); // light blue
rgb color2 = RGB(0.5,0.4,0.6); // white
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

rgb bg1 = RGB(0,0,0);
rgb bg2 = RGB(1,0.95,0.9);

f32 circle(vec2 p, vec2 center, f32 radius){
  return vec2_len(vec2_sub(p, center)) - radius;
}

rgb gradient(vec2 p, rgb color1, rgb color2, vec2 start, vec2 stop){
  vec2 lvec = vec2_sub(stop, start);
  f32 ratio = vec2_dot(vec2_sub(p, start), lvec) / vec2_dot(lvec, lvec);
  ratio = CLAMP(ratio,0,1.0);
  f32 n = noise(p);
  ratio = ratio + 5 * (n - 0.5f) / 128.0f;
  rgb c = rgb_blend(color1, color2, ratio);
  return c;
}

rgb gradient_circle(vec2 p, rgb bg, rgb color1, rgb color2, vec2 p1, vec2 p2, vec2 center, f32 radius){
  // (P - P1) dot (P - P2)
  // Linearily interpolate the colors.
  f32 d = circle(p, center, radius);
  if(d <= pixel_size * 2){
    rgb c = df_mix(gradient(p, color1, color2, p1, p2), bg, d);
    return c;
  }
  return bg;
}
f32 square(vec2 p, vec2 center, vec2 radius){
  p = vec2_abs(vec2_sub(p, center));
  if(p.x > radius.x){
    if(p.y > radius.y)
      return vec2_len(vec2_sub(p, radius));
    return p.x - radius.x;
  }else if(p.y > radius.y)
    return p.y - radius.y;
  // inside the square.
  return fmax(p.y- radius.y, p.x - radius.x);
}

vec2 square1;
rgb square1_color;
vec2 square1_size;
bool loaded = false;
void load(){
  if(loaded) return;
  vec2 square0 = vec2_new(0.75,0.8);
  square1_color = gradient(square0, color1, color2, p1, p2);
  square1_size = vec2_new(0.025, 0.1);
  square1 = vec2_sub(square0, square1_size);
  loaded = true;
}

rgb get_color(vec2 p){
  load();
  rgb bg = gradient(p, bg1, bg2, vec2_new(1.0,1.0), vec2_new(-1.5,-1.5));
  rgb bg3 = gradient(p, bg2, black, vec2_new(0.5f,1.0), vec2_new(0.5,-1.5));
  bg = rgb_add(bg, bg3);
  
  rgb a = gradient_circle(p, bg, color0, color2, p1, p2, center1, radius1);
  a = rgb_blend(a, bg, 0.8);
  rgb b = gradient_circle(p, a, color3, color2, p1, p2, center2, radius2);
  b = rgb_blend(b, bg, 0.8);
 
  rgb c = gradient_circle(p, b, color1, color2, p1, p2, center3, radius3);
  f32 sq = square(p, square1, square1_size) - 0.01;
  rgb d = df_mix(square1_color, c, sq);
  return d;
  
}
