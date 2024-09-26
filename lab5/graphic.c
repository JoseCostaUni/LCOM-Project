#include "graphic.h"
#include <machine/int86.h>
#include <lcom/lcf.h>
#include "kbc.h"
#include "i8042.h"
#include "VBE.H"
#include <stdint.h>
#include <stdio.h>
#include "keyboard.h"

uint8_t *drawBuffer;
unsigned bytesPerPixel;
uint16_t xRes, yRes;
vbe_mode_info_t mode_info;
uint8_t* vAddr_base;

/*
Parâmetro reg86_t 	Text Mode 	Graphic Mode
ah 	                  0x00 	        0x4F
al 	                  Ox03 	        0x02
ax 	                  0x0003 	      0x4F02
bx 	                  0x0000 	submode | BIT(14)
intno 	              0x10 	        0x10

*/
int (set_graphic_mode)(uint16_t mode){
  reg86_t reg86;

  memset(&reg86 , 0 , sizeof(reg86));

  reg86.ah = 0x4F;
  reg86.al = 0x02;
  reg86.intno = 0x10;
  reg86.bx = mode | BIT(14);

  if(sys_int86(&reg86) != 0 ){
    return 1;
  }

  return 0;
}

int (set_text_mode)(){
  
  return 0;
}

int (set_frame_mode)(uint16_t* mode){

  memset(&mode_info , 0 , sizeof(mode_info));

  if(vbe_get_mode_info(*mode , &mode_info) != 0){
    return 1;
  }

  int r;

  struct minix_mem_range mr;

  bytesPerPixel = (mode_info.BitsPerPixel* 7)  /8;

  unsigned int vram_size = (mode_info.XResolution * mode_info.YResolution) * bytesPerPixel;

  mr.mr_base = mode_info.PhysBasePtr;
  mr.mr_limit = vram_size + mr.mr_base;

  if(OK != (r = sys_privctl(SELF,SYS_PRIV_ADD_MEM,&mr))){
    panic("sys_privctl (Physical memory alocation error) failed: %d\n",r);
    return 1;
  }

  drawBuffer = vm_map_phys(SELF , (void *) mr.mr_base , vram_size);

  if(drawBuffer == MAP_FAILED){
    return 1;
  }

  return 0;
}

int (get_h_res)(){
  return mode_info.XResolution;
}
int (get_v_res)(){
  return mode_info.YResolution;
}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color){

  if(x > mode_info.XResolution || y > mode_info.YResolution || x < 0 || y < 0){
    printf("invalid coordiates");
    return 1;
  }

  unsigned bytesPerPixel = (mode_info.BitsPerPixel + 7) / 8;

  uint8_t coordinates = (mode_info.XResolution * y) + x;

  if(memcpy(&drawBuffer[coordinates] , &color , bytesPerPixel) == NULL){
    return 1;
  }

  return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){

  for(int i = x ; i < len ; i++){
    vg_draw_pixel(i+x , y , color);
  }
  
  return 0;
}

int (vg_draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){

  for(int i = y ; i < len ; i++){
    vg_draw_pixel(x , i+y , color);
  }

  return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y,uint16_t width, uint16_t height, uint32_t color){
  
  for(uint16_t i = 0; i < height;i++){
    if(draw_line(x,y+i,color,width) != 0){
      printf("Failed to draw line");
      return 1;
    }
  }
  return 0;

}

int (normalize_color)(uint32_t color, uint32_t* new_color){

  if(mode_info.BitsPerPixel == 32){
    *new_color = color;
  }else{
    int bbp = mode_info.BitsPerPixel;
    uint32_t mask = BIT(bbp) -1 ;
    *new_color = mask & color;
  }
  
  return 0;
}

int (wait_for_ESC_)(){

  if(wait_for_ESC() != OK){
    return 1;
  }

  return 0;
}
