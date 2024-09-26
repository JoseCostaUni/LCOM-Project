#include <lcom/lcf.h>
#include <lcom/lab3.h>
#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"
#include "keyboard.h"
#include "kbc.h"

int hook_id = 1;
uint8_t scancode;
struct scancode_info scan_info;

int (keyboard_subscribe_int)(uint8_t *bit_no) {

  *bit_no = hook_id;

  if(sys_irqsetpolicy(IRQ_KEYBOARD ,IRQ_REENABLE_EXCLUSIVE , &hook_id) != OK){
    return 1;
  }
 
  return 0;
}

int (keyboard_unsubscribe_int)() {
  
  if(sys_irqrmpolicy(&hook_id) != OK){
    return 1;
  }

  return 0;
}

int (read_status_register)(uint8_t *st){

  if(util_sys_inb(STATUS_BYTE , *st) != OK)
    return 1;

  return 0;
}

int (read_out_buffer)(uint8_t *out){
  
  if(util_sys_inb(OUT_SCANCODES , *out) != OK)
    return 1;

  return 0;
}

void (kbc_ih)() {

  //a cada interrupt vamos ler o status byte e se tiver cheio lemos o outbuffer

  uint8_t st;
  read_status_register(&st);

  if((st & OUT_BUFF_FULL) == 1){
    read_out_buffer(&scancode);
  }

  if((st & KEYBOARD_STATUS_ERRORS) == 1){
    scancode = -1;
  }
}

/*
struct scancode_info {
  uint8_t size_counter;
  bool make_code;
  uint8_t bytes[2];
};*/
void(clean_scan_info)(struct scancode_info *scan){
  for(int i = 0; i < scan->size_counter ; i++){
    scan->bytes[i] = 0;
  }
  scan->make_code = false;
  scan->size_counter = 0;
}

bool(receive_keyboard_scan)(struct scancode_info *scan_info, uint8_t *scancode){

  bool is_E0 = false;
  bool valid = true;

  if(*scancode == ESC_BREAK_CODE){
    scan_info->size_counter = 1;
    scan_info->make_code = false;
    scan_info->bytes[0] = ESC_BREAK_CODE;
  }else{
    if(*scancode == 0xE0){
      is_E0 = true;
      scan_info->size_counter = 1;
      scan_info->make_code = true;
      scan_info->bytes[0] = *scancode;
    }else{
      if((*scancode >> 7) & 1){ // ver se se msb é break code
        scan_info->make_code = false;
      }else{
        scan_info->make_code = true;
      }
      scan_info->bytes[scan_info->size_counter] = *scancode;
      scan_info->size_counter++;
    }
  }

  if(is_E0){
    kbd_print_scancode(scan_info->make_code , scan_info->size_counter , scan_info->bytes);
  }


  if(valid){
    return true;
  }else{
    return false;
  }
}

int(kbc_enable_interrupts)(){

  uint8_t command_byte;

  if(kbc_write_command(READ_COMMAND_BYTE)){
    return 1;
  }    
  
  if(try_read_out_buffer(&command_byte)){
    return 1;
  }

  command_byte |= ENABLE_INTERRUPT_OBF_KEYBOARD;


  if(kbc_write_command(WRITE_COMMAND_BYTE)){
    return 1;
  }


  if(kbc_write_command_args(&command_byte)){
    return 1;
  }

  return 0;
}

int try_read_out_buffer(uint8_t *out){

    uint8_t st;
    uint8_t temp_out;
    while (1)
    {
        read_status_register(&st);
        if((st & OUT_BUFF_FULL) == 1){
            read_out_buffer(&temp_out); 
            if((st & KEYBOARD_STATUS_ERRORS) == OK){
                *out = temp_out;
                return 0;
            }else{
              *out = -1;
              return 1;
            }
        }
        tickdelay(micros_to_ticks(DELAY_US));
    }
    

    return 1;
}

bool(test_status_polling)(uint8_t st){

  if((st & OUT_BUFF_FULL) || (st & AUX_STATUS_REG == 0))
    return true;

  return false;
}
