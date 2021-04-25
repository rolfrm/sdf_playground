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
  if(r <= 0.0) return vec2_len(p1);
  if(r >= 1.0) return vec2_len(vec2_sub(p, b));
  return vec2_len(vec2_sub(p, vec2_add(a, vec2_scale(ab, r))));
}

f32 vec2_dist(vec2 a, vec2 b){
  return vec2_len(vec2_sub(a, b));
}

vec2 vec2_interp(vec2 a, vec2 b, f32 t){
  return vec2_add(vec2_scale(a, (1 - t)), vec2_scale(b, t));
}

vec2 curve_pt(vec2 a, vec2 b, vec2 c, f32 t){
  vec2 l1 = vec2_interp(a,b,t);
  vec2 l2 = vec2_interp(b,c,t);
  return vec2_interp(l1, l2, t);
}

f32 curve_dist3(vec2 p, vec2 a, vec2 b, vec2 c, f32 i1, f32 i2, f32 d1, f32 d2){
  f32 t3 = 0.5f * (i1 + i2);
  vec2 mid = curve_pt(a,b,c,t3);
  vec2 a2 = curve_pt(a,b,c,i1);
  vec2 b2 = curve_pt(a,b,c,i2);
  
  f32 d3 = vec2_dist(p, mid);
  f32 da = line_dist(p, a2, mid);
  f32 db = line_dist(p, mid, b2);
  if(fabs(da - db) < pixel_size * 0.01) return da;
  if(fabs(da - db) < 0.001){
    return MIN( curve_dist3(p, a, b, c, i1, t3, d1, d3),
		curve_dist3(p, a, b, c, t3, i2, d3, d2));
  }

  if(da < db)
    return curve_dist3(p, a, b, c, i1, t3, d1, d3);
  return curve_dist3(p, a, b, c, t3, i2, d3, d2);
  
}

f32 curve_dist2(vec2 p, vec2 a, vec2 b, vec2 c){
  f32 a1 = vec2_dist(p, a);
  f32 b1 = vec2_dist(p, b);
  vec2 mid = curve_pt(a,b,c, 0.5f);
  f32 c1 = vec2_dist(p, mid);
  f32 d1 = curve_dist3(p, a, b, c, 0.0f, 1.0f, a1, b1);
  //f32 d2 = curve_dist3(p, a, b, c, 0.5f, 1.0f, c1, b1);
  return d1;//MIN(d1,d2);//MIN3(a1, b1, c1);
  
}

f32 curve_dist(vec2 p, vec2 a, vec2 b, vec2 c){
  // a bezier curve is a somewhate recursively defined line
  // l1 = (1 - t) * p0 + t * p1
  // l2 = (1 - t) * p1 + t * p2
  // l3 = (1 - t) * l1 + t * l2
  /*f32 minD = 10000;
  vec2 found;
  for(int i = 0; i < 32; i++){
    vec2 h2 = curve_pt(a,b,c, i / 32.0f);
    f32 d1 = vec2_dist(p, h2);
    if(d1 < minD){
      minD = d1;
    }
  }
  return minD;*/
  int it = 0;
  f32 loc = 0.5;
  f32 d = 0.5;
 start:
  it += 1;
  d *= 0.5;
  vec2 h1 = curve_pt(a,b,c, loc);
  f32 d0 = vec2_dist(p, h1);
  if(it == 16)
    return d0;
  vec2 h2 = curve_pt(a,b,c, loc - 0.00001f);
  vec2 h3 = curve_pt(a,b,c, loc + 0.00001f);
  
  f32 d1 = vec2_dist(p, h2);
  f32 d2 = vec2_dist(p, h3);
  if(d0 < d1 && d0 < d2)
    return d0;
  f32 dt = d;
  if(d1 < d2){
    dt = -d;
  }
  loc += dt;
  goto start;
  return 0.0;
}

//https://www.iquilezles.org/www/articles/distfunctions2d/distfunctions2d.htm
f32 sdBezier(vec2 pos, vec2 A, vec2 B, vec2 C )
{    
  vec2 a = vec2_sub(B, A);
  vec2 b = vec2_add(vec2_sub(A, vec2_scale(B, 2.0*), C));
  vec2 c = vec2_scale(a, 2.0);
  vec2 d = vec2_sub(A, pos);
  float kk = 1.0/dot(b,b);
  float kx = kk * dot(a,b);
  float ky = kk * (2.0*dot(a,a)+dot(d,b)) / 3.0;
  float kz = kk * dot(d,a);      
  float res = 0.0;
  float p = ky - kx*kx;
  float p3 = p*p*p;
  float q = kx*(2.0*kx*kx-3.0*ky) + kz;
  float h = q*q + 4.0*p3;
  if( h >= 0.0) 
    { 
      h = sqrt(h);
      vec2 x = vec2_scale(vec2_sub(vec2_new(h,-h), q), 0.5);
      vec2 uv = vec2_new(sign(x.x)*pow(abs(x.x), 1.0/3.0),
			 sign(x.y)*pow(abs(x.y), 1.0/3.0)
			 );
      float t = CLAMP( uv.x+uv.y-kx, 0.0, 1.0 );
      let tmp = vec2_add(d, vec2_scale(vec2_add(c, vec2_scale(b, t)), t));  
      res = dot2(tmp);
    }
    else
    {
        float z = sqrt(-p);
        float v = acos( q/(p*z*2.0) ) / 3.0;
        float m = cos(v);
        float n = sin(v)*1.732050808;
        vec3  t = clamp(vec3(m+m,-n-m,n-m)*z-kx,0.0,1.0);
        res = min( dot2(d+(c+b*t.x)*t.x),
                   dot2(d+(c+b*t.y)*t.y) );
        // the third root cannot be the closest
        // res = min(res,dot2(d+(c+b*t.z)*t.z));
    }
    return sqrt( res );
}

rgb pseudo_color(f32 x){
  static rgb col1 = RGB(1,0.6,0.5);
  static rgb col2 = RGB(0.5,0.6,1.0);
  x = sin(x / pixel_size *0.8f) * 0.9 + 0.7;
  return rgb_blend(col1, col2, x);
}

rgb get_color(vec2 p){
  static bool first = false;
  if(first == false){
    first = true;
    vec2 p2 = curve_pt(vec2_new(0.2,0.2), vec2_new(0.3,0.5), vec2_new(0.5,0.5), 0.25);
    //p2.x += 0.001;
    f32 d3 = curve_dist2(p2, vec2_new(0.2,0.2), vec2_new(0.3,0.5), vec2_new(0.5,0.5));
    logd("%f \n", d3);
  }
  /*f32 d4 = line_dist(p, vec2_new(0.1,0.5), vec2_new(0.9, 0.5)) - 0.01;
  f32 d5 = line_dist(p, vec2_new(0.1,0.7), vec2_new(0.9, 0.7)) - 0.01;
  f32 d6 = line_dist(p, vec2_new(0.1,0.3), vec2_new(0.9, 0.3)) - 0.01;
  f32 d7 = line_dist(p, vec2_new(0.5,0.3), vec2_new(0.5, 0.7)) - 0.01;

  f32 d = MIN3(d5,d4, d6);
  d = MIN(d7, d);*/
  f32 d = curve_dist2(p, vec2_new(0.2,0.4), vec2_new(0.3,-0.3), vec2_new(0.9,0.9)) - 0.001;
  rgb b = pseudo_color(d);
  return df_mix(black, b, d);
}
