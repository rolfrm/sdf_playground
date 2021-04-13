#include <iron/full.h>

static const f32 NEGATIVE_INFINITY = -1.0f/0.0f;

typedef enum{
	     DITHER_NONE,
	     DITHER_FLOYD_STEINBERG
}DITHERING;

extern DITHERING dithering;

typedef struct{
  // expensive 32-bit color channels. oh well.
  f32 r, g, b;
}rgb;

rgb rgb_blend(rgb a, rgb b, f32 ratio);



f32 get_distance(vec2 p);
rgb get_color(vec2 p);

extern f32 pixel_size;
