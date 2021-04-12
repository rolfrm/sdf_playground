#include <iron/full.h>
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
