#include <distance_fields.h>

//#define DARK 0

#ifdef DARK
rgb c1 = RGB(0.5,0.5,0.5);
rgb c2 = RGB(0.9, 0.5, 0.5);
rgb c3 = RGB(0.0, 0.1, 0.2);
#else
rgb c1 = RGB(1,1,1);
rgb c2 = RGB(0.5, 0.9, 0.9);
rgb c3 = RGB(0.1, 0.2, 0.3);
#endif
vec2 p1 = VEC2(0,0);
vec2 p2 = VEC2(0,1);


vec2 points[] = {
		 VEC2(0.5, 0.6),
		 VEC2(0.5, 0.8),
		 VEC2(0.7, 0.3),
		 VEC2(0.3, 0.3),
		 VEC2(0.5, 0.2)

};

rgb get_color(vec2 p){
  rgb color1 = df_gradient(p, c1, c2, p1, p2);
  rgb color2 = df_gradient(p, c2, c3, p1, p2);
  f32 d = polygon_distance(p, points, array_count(points));
  return df_mix(color2, color1, d);

  
  ///f32 d = get_distance(p);
  //return df_mix(color1, background, d);
}

