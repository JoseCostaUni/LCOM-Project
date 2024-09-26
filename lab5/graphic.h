#include <machine/int86.h>
#include "lcom/lcf.h"
#include "VBE.H"



/**
 * @brief This is the base addres (pointer) to the beggining of th array where display pixels are stored
 */
//int (vg_exit)(); dado por LCF

int (set_graphic_mode)(uint16_t mode);
int (set_text_mode)();

int (get_h_res)();
int (get_v_res)();

int (set_frame_mode)(uint16_t* mode);
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (vg_draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (vg_draw_rectangle)(uint16_t x, uint16_t y,uint16_t width, uint16_t height, uint32_t color);

int (normalize_color)(uint32_t color, uint32_t* new_color);

int (wait_for_ESC_)();

uint32_t (direct_mode)(uint32_t R, uint32_t G, uint32_t B);
uint32_t (indexed_mode)(uint16_t col, uint16_t row, uint8_t step, uint32_t first, uint8_t n);
uint32_t (Red)(unsigned j, uint8_t step, uint32_t first);
uint32_t (Green)(unsigned i, uint8_t step, uint32_t first);
uint32_t (Blue)(unsigned j, unsigned i, uint8_t step, uint32_t first);
uint32_t (R)(uint32_t first);
uint32_t (G)(uint32_t first);
uint32_t (B)(uint32_t first);