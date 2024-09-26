#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

uint32_t counter = 0;
int32_t hook_id = TIMER0_IRQ;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  //mudar frequencia, manter config atual
  //1. ler config (done)
  //2. construir config
  //3. Calcular valor de init (done) linha 15

  uint8_t st , msb , lsb;
  int finalFreq = TIMER_FREQ/freq;

  if(timer_get_conf(timer , &st) != OK)
    return 1;

  switch (timer)
  {
  case 1:
    st &= 0x0F;
    st |= TIMER_SEL0;
    break;
  case 2:
    st &= 0x0F;
    st |= TIMER_SEL1;
    break;
  case 3:
    st &= 0x0F;
    st |= TIMER_SEL2;
    break;
  default:
    break;
  }

  st |= TIMER_LSB_MSB;

  if(sys_outb(TIMER_CTRL , st) != OK)
    return 1;

  if(util_get_LSB(finalFreq , &lsb) != OK)
    return 1;

  if(sys_outb(TIMER_0 + timer , lsb) != OK)
    return 1;

  if(util_get_MSB(finalFreq , &msb) != OK)
    return 1;

  if(sys_outb(TIMER_0 + timer , msb) != OK)
    return 1;

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
   //Para subscrever um interrupt usamos o comando sys_irqsetpolicy
   //1º Argumento = IRQ do timer que é 0
   //2º Argumento = IRQ_REENABLE reativa o irq depois do interrupt
   //3º Argumento = hook_id

  *bit_no = hook_id;

  if(sys_irqsetpolicy(TIMER0_IRQ , IRQ_REENABLE , &hook_id) != OK)
    return 1;
 
  return 0;
}

int (timer_unsubscribe_int)() {
  
  if(sys_irqrmpolicy(&hook_id) != OK)
    return 1;
 
  return 0;
}

void (timer_int_handler)() {
  counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {

  uint8_t rb_command = TIMER_RB_CMD;

  rb_command |= TIMER_RB_COUNT_;
  rb_command |= TIMER_RB_SEL(timer);

  if(sys_outb(TIMER_CTRL , rb_command) != OK)
    return 1;

  if(util_sys_inb(TIMER_0 + timer , &st) != OK)
    return 1;

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,enum timer_status_field field) {
  
  union timer_status_field_val val;
  
  switch (field)
  {
  case tsf_all:
    val.byte = st;
    break;
  case tsf_initial: 
    st = st & TIMER_LSB_MSB;

    st >>= 4;

    switch (st)
    {
    case 1:
      
      val.bcd = LSB_only;

      break;
    case 2:
      val.bcd = MSB_only;
      break;
    case 3:
      val.bcd = MSB_after_LSB;
      break;
    default:
      break;
    }

    break;
  case tsf_mode:

    if((st & TIMER_OP_EXTRACT) == 2){
      val.count_mode = TIMER_RATE_GEN;
    }else if((st & TIMER_OP_EXTRACT) == 3){
      val.count_mode = TIMER_SQR_WAVE;
    }

    break;
  case tsf_base:

    if((st & TIMER_BCD) == 1){
      val.bcd = true;
    }else{
      val.bcd = false;
    }

    break;
  
  default:
    break;
  }

  if(timer_print_config(timer , field , val) != OK)
    return 1;

  return 0;
}
