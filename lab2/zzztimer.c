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

  uint8_t st,lsb,msb;
  uint32_t freqFinal = TIMER_FREQ / freq;

  if(timer_get_conf(timer,&st)){
    return 1;
  }

  switch (timer) {
  case 0:
    st &= 0x0F;
    st |= TIMER_SEL0; // Máscara com 0 e 0 no bit 6 e 7 respetivamente
    break;

  case 1:
    st &= 0x0F;
    st |= TIMER_SEL1; //Máscara com 1 e 0 no bit 6 e 7 respetivamente
    break;
  
  case 2:
    st &= 0x0F;
    st |= TIMER_SEL2; //Máscara com 0 e 1 no bit 6 e 7 respetivamente
    break;

  default:
    break;
  }

  st |= TIMER_LSB_MSB; // Coloca a 1 e 1 o bit 4 e 5 , significa que o MSB vai ser lido após o LSB

  if(sys_outb(TIMER_CTRL,st)){
    return 1;
  }

  if(util_get_LSB(freqFinal,&lsb)){
    return 1;
  }

  if(sys_outb(TIMER_0+timer,lsb)){
    return 1;
  }

  if(util_get_MSB(freqFinal,&msb)){
    return 1;
  }

  if(sys_outb(TIMER_0+timer,msb)){
    return 1;
  }

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
   //Para subscrever um interrupt usamos o comando sys_irqsetpolicy
   //1º Argumento = IRQ do timer que é 0
   //2º Argumento = IRQ_REENABLE reativa o irq depois do interrupt
   //3º Argumento = hook_id

   *bit_no = hook_id;

   if(sys_irqsetpolicy(TIMER0_IRQ,IRQ_REENABLE,&hook_id) != 0){
    return 1;
   }

  return 0;
}

int (timer_unsubscribe_int)() {
  
  if(sys_irqrmpolicy(&hook_id) != 0){
    return 1;
  }

  return 0;
}

void (timer_int_handler)() {
  counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  int port; // Has the port from which we will get the config
            // receives it's value from timer since it ranges from 0 to 2

  uint8_t rb_command = TIMER_RB_CMD; // BIT(6) | BIT(7) Para dizer que é Read-Back Command
  rb_command = rb_command | TIMER_RB_COUNT_; // BIT(5) Perguntar se como lê em 0, tenho de trocar   o valor.
  rb_command = rb_command | TIMER_RB_SEL(timer); //Perguntar o mesmo

  if(sys_outb(TIMER_CTRL,rb_command)){
    return 1;
  }
  
  port = TIMER_0 + timer;


  if(util_sys_inb(port,st)) //Calls util_sys_inb
    return 1;

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,enum timer_status_field field) {
  
  union timer_status_field_val val;

  switch (field)
  {
  case tsf_all: // status byte

    val.byte = st; // Select which timer to use , normally number 2

    break;

  case tsf_initial: // Timer initialization mode

      st = st & TIMER_LSB_MSB;
      st >>= 4;

      switch (st){

      case 1:
        val.in_mode = LSB_only;
        break;

      case 2:
        val.in_mode = MSB_only;
        break;

      case 3:
        val.in_mode = MSB_after_LSB;
        break;

      default:
        val.in_mode = INVAL_val;
        break;
      }

    break;

  case tsf_mode: // Counting mode
    st = st & TIMER_OP_EXTRACT;

    switch (st) {
    case 2:
      val.count_mode = TIMER_RATE_GEN;
      break;
    case 3:
      val.count_mode = TIMER_SQR_WAVE;
    default:
      break;
    }
    break;

  case tsf_base: //Counting base, true if BCD

    if(st & TIMER_BIN){

      val.bcd = false; // False, is used for binary counting mode

    }else if(st & TIMER_BCD){

      val.bcd = true; // True, is used for BCD counting mode

    }
    break;
  
  default:

    return 1;

    break;
  }

  if(timer_print_config(timer,field,val)){
    return 1;
  }

  return 0;
}
