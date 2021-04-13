#include "distance_fields.h"
#include <png.h>
//#define CLAMP(v, min, max) (v < min ? min : (v > max ? max : v))

DITHERING dithering;

rgb rgb_blend(rgb a, rgb b, f32 ratio){
  f32 inv_ratio = 1.0f - ratio;
  return (rgb){
    .r = a.r * ratio + b.r * inv_ratio,
      .g = a.g * ratio + b.g * inv_ratio,
      .b = a.b * ratio + b.b * inv_ratio};
}


void write_png_file_rgb(const char *filename, const int width, const int height, int bit_depth, rgb (* f) (int x, int y, void * userdata), void * userdata) {

  FILE *fp = fopen(filename, "wb");
  if(!fp)
    ERROR("Could not open '%s'", filename);

  png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png) ERROR("Could not write PNG version string");

  png_infop info = png_create_info_struct(png);
  if (!info)ERROR("Could not create png struct");

  if (setjmp(png_jmpbuf(png)))
    ERROR("error writing png");

  png_init_io(png, fp);
  int type = PNG_COLOR_TYPE_RGB;
  png_set_IHDR(
	       png,
	       info,
	       width, height,
	       bit_depth, // 1, 2, 4, 8, 16
	       type,
	       PNG_INTERLACE_NONE,
	       PNG_COMPRESSION_TYPE_DEFAULT,
	       PNG_FILTER_TYPE_DEFAULT
	       );
  
  png_write_info(png, info);
  if(bit_depth == 8){
    u8 data[width * 3];
    for(int j = 0; j < height; j++){
      
      for(int i = 0; i < width; i++){
	rgb color = f(i,j,userdata);
	u8 * c = data + i * 3;
	c[0] = (u8) CLAMP(color.r * 255, 0, 255);
	c[1] = (u8) CLAMP(color.g * 255, 0, 255);
	c[2] = (u8) CLAMP(color.b * 255, 0, 255);
      }
      // To remove the alpha channel for PNG_COLOR_TYPE_RGB format,
      // Use png_set_filler().
      //png_set_filler(png, 0, PNG_FILLER_AFTER);
      png_write_row(png, data);
    }
  }
 else if(bit_depth == 16){
   png_set_swap(png);
   const u16 u16_max = 0xFFFF;
   u16 data[width * 3];
    for(int j = 0; j < height; j++){
      
      for(int i = 0; i < width; i++){
	rgb color = f(i,j,userdata);
	u16 * c = data + i * 3;
	c[0] = (u16) CLAMP(color.r * u16_max, 0, u16_max);
	
	c[1] = (u16) CLAMP(color.g * u16_max, 0, u16_max);
	c[2] = (u16) CLAMP(color.b * u16_max, 0, u16_max);
      }
      // To remove the alpha channel for PNG_COLOR_TYPE_RGB format,
      // Use png_set_filler().
      //png_set_filler(png, 0, PNG_FILLER_AFTER);
      png_write_row(png, (void *)data);
    }
  }
  png_write_end(png, NULL);
  fclose(fp);
}

rgb color_it_red(int x, int y, void * userdata){
  return (rgb){.r = 0.0, .g = 1.0, .b = 0.0};
}
typedef struct{
  vec2 start, stop;
  vec2 size;
  f32 pixel_size;
  rgb background;
  f32 aspect;
}distfield_ctx;
f32 pixel_size;
rgb color_distfield(int x, int y, void * userdata){
  distfield_ctx * ctx = userdata;
  
  vec2 p = vec2_div(vec2_new(x, y), ctx->size);
  vec2 d = vec2_sub(ctx->stop, ctx->start);
  vec2 p2 = vec2_add(vec2_mul(p, d), ctx->start);
  f32 dist = get_distance(p2);
  rgb bg = ctx->background;
  pixel_size = ctx->pixel_size;
  if(dist < 0.0){
    return get_color(p2);
  }else if(dist < ctx->pixel_size){
    return rgb_blend(bg, get_color(p2), dist / ctx->pixel_size); 
  }
  return bg;
}


void distfield_calc_aspect(distfield_ctx * ctx){
  ctx->aspect = ctx->size.x / ctx->size.y;
  if(ctx->aspect > 1.0){
    f32 d = (ctx->stop.x - ctx->start.x) * (ctx->aspect - 1) * 0.5;
    ctx->stop.x +=d;
    ctx->start.x -= d;
  }else{
    f32 d = (ctx->stop.y - ctx->start.y) * (1.0 - ctx->aspect) * 0.5;
    ctx->stop.y +=d;
    ctx->start.y -= d;
  }
}

int main(int c, char ** argv){
  for(int i = 0; i < c; i++){
    logd("%i: %s\n",i , argv[i]);
  }
  int width = 512, height = 512;
  if(c == 1){
    logd("usage: [file] [width] [height]\n");
    return -1;
  }
  char * file = argv[1];
  if(c >=3)
    sscanf(argv[2], "%i", &width);
  if(c >= 4)
    sscanf(argv[3], "%i", &height);
  if(width > 100000 || height > 100000){
    logd("Invalid dimensions %i %i\n", width, height);
  }
  logd("%s %i %i\n", file, width, height);
  
  float px_size1 = 1.0 / width;
  float px_size2 = 1.0 / height;
  
  distfield_ctx ctx = {
		       .start = vec2_new(0,0),
		       .stop = vec2_new(1,1),
		       .background = (rgb){.r = 1.0, .g = 1.0, .b = 1.0},
		       .pixel_size = sqrtf(px_size1 * px_size1 + px_size2 * px_size2),
		       .size = vec2_new(width, height)
  };
  distfield_calc_aspect(&ctx);

  write_png_file_rgb(file, width, height, 16, color_distfield, &ctx);
  return 0;
  
}
