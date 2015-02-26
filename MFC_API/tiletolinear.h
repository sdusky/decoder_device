#ifndef TILETOLINER_H_
#define TILETOLINER_H_

static void tile_to_linear_64x32_4x2_neon(unsigned char *p_linear_addr, unsigned char *p_tiled_addr, unsigned int x_size, unsigned int y_size);
static void tile_to_linear_64x32_4x2_uv_neon(unsigned char *p_linear_addr, unsigned char *p_tiled_addr, unsigned int x_size, unsigned int y_size);

#endif
