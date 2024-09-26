#include <lcom/lcf.h>

#include <stdint.h>


//int sys_int86(struct reg86 *reg86p);

uint32_t cnt;

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb = (uint8_t) val;
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  val = val >> 8;
  *msb = (uint8_t) val;
  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {

  uint32_t val;

  if(sys_inb(port,&val) != 0){
    return 1;
  }

  *value = (uint8_t) val;

  #ifdef LAB3
  cnt++;
  #endif

  return 0;
}
