#include "lcom/lcf.h"
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

  if(sys_irqsetpolicy(IRQ_KEYBOARD , 0x001 | 0x002 , &hook_id) != OK)
    return 1;

  return 0;
}

int (keyboard_unsubscribe_int)() {

  if(sys_irqrmpolicy(&hook_id) != OK)
    return 1;

  return 0;
}

int (read_status_register)(uint8_t *st){
  
  if(util_sys_inb(STATUS_BYTE , st) != OK)
    return 1;

  return 0;
}

int (read_out_buffer)(uint8_t *out){


  if(util_sys_inb(OUT_SCANCODES , out) != OK)
    return 1;

  return 0;
}

void (kbc_ih)() {

  uint8_t st;

  read_status_register(&st);

  if((st & OUT_BUFF_FULL) == 1){
    read_out_buffer(&scancode);
  }

  if((st & KEYBOARD_STATUS_ERRORS) == 1){
    scancode = -1;
  }
}

bool(test_status_polling)(uint8_t st){
  if((st & AUX_STATUS_REG) == 0 && (st & OUT_BUFF_FULL)){
    return true;
  }

  return false;
}
/*
struct scancode_info {
  uint8_t size_counter;
  bool make_code;
  uint8_t bytes[2];
;*/
void(clean_scan_info)(struct scancode_info *scan){
  for(int i = 0 ; i < scan->size_counter ; i++){
    scan->bytes[i] = 0;
  }
  scan->size_counter = 0;
  scan->make_code = false;
}

bool(receive_keyboard_scan)(struct scancode_info *scan_info, uint8_t *scancode){
  bool is_E0 = false;
  bool valid = true;

  if(*scancode == ESC_BREAK_CODE){
    scan_info->make_code = false;
    scan_info->size_counter = 1;
    scan_info->bytes[0] = ESC_BREAK_CODE;
    valid = false;
  }else{
    if(*scancode == 0xE0){
      scan_info->bytes[0] = *scancode;
      scan_info->size_counter++;
      is_E0 = true;
    }else{
      if((*scancode >> 7) & 1){
        scan_info->make_code = false;
      }else{
        scan_info->make_code = true;
      }

      scan_info->bytes[scan_info->size_counter] = *scancode;
      scan_info->size_counter++;
    }
  }

  if(!is_E0){
    clean_scan_info(scan_info);
  }

  if(valid){
    return true;
  }else{
    return false;
  }
}

int(kbc_enable_interrupts)(){
  
  uint8_t command_byte;

  if(kbc_write_command(READ_COMMAND_BYTE) != OK){
    return 1;
  }

  if(try_read_out_buffer(&command_byte)){
    return 1;
  }

  command_byte |= ENABLE_INTERRUPT_OBF_KEYBOARD;

  if(kbc_write_command(WRITE_COMMAND_BYTE) != OK){
    return 1;
  }

  if(kbc_write_command_args(&command_byte) != OK){
    return 1;
  }

  return 0;
}

int try_read_out_buffer(uint8_t *out){
  uint8_t st , temp_out;

  while (1)
  {
    read_status_register(&st);

    if((st & OUT_BUFF_FULL)){
      read_out_buffer(&temp_out);
      if((st & KEYBOARD_STATUS_ERRORS)){
        *out = -1;
        return 1;
      }else{
        *out = temp_out;
        return 0;
      }
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }
}


int (wait_for_ESC)(){
    int ipc_status,r;
  message msg;
  bool valid = true;

  uint8_t irq_set;

  if(keyboard_subscribe_int(&irq_set) != 0){
    return 1;
  }

  while(valid) { 
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }

    if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
              case HARDWARE:			
                if (msg.m_notify.interrupts & BIT(irq_set)) { 
                  kbc_ih(); 

                  if(!receive_keyboard_scan(&scan_info,&scancode)){
                    valid = false;
                  }
                }
                break;
            }
    }
 }

  if(keyboard_unsubscribe_int() != 0){
    return 1;
  }

  return 0;
}






