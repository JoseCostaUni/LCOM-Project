#ifndef _MOUSE_H_
#define _LCOM_I8254_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include "i8042.h"

int(mouse_subscribe_int)(uint8_t *bit_no);
int(mouse_unsubscribe_int)();
int (read_status_register)(uint8_t *st);
int(reset_mouse_struct)(struct packet *mouse);
int(parse_values)(uint8_t data,int *cnt,struct packet *pp);
int(disable_mouse_report)();
int(enable_mouse_report)();
void clean_packet(struct packet *mouse);
int (mouse_gesture)(uint8_t x_len,uint8_t tolerance);

enum States{
  INITIAL,
  DRAWING_FIRST_LINE,
  DRAWING_VERTEX_ZONE,
  DRAWING_SECOND_LINE,
  COMPLETE,
};



#endif

