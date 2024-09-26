/**
 * @file timer.c
 * @brief File containing the implementation of the functions to interact with the Timer
 */
#include <lcom/lcf.h>
#include "timer.h"

#include <stdint.h>

#include "i8254.h"

uint32_t counter = 0;
int32_t __hook_id = 0;

/**
 * @brief Sets the frequency of the timer
 * 
 * This function sets the frequency of the timer
 * 
 * @param timer Timer to be set
 * @param freq Frequency to be set
 * 
 * @return int 0 upon success, 1 otherwise
 */
int (timer_set_frequency)(uint8_t timer, uint32_t freq) {

  uint8_t st , msb , lsb;
  int finalFreq = TIMER_FREQ/freq;

  printf("finalFreq: %d\n" , finalFreq);

  if(timer_get_conf(timer , &st) != OK)
    return 1;

  switch (timer)
  {
  case 0:
    st &= 0x0F;
    st |= TIMER_SEL0;
    break;
  case 1:
    st &= 0x0F;
    st |= TIMER_SEL1;
    break;
  case 2:
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

/**
 * @brief Subscribes Timer interrupts
 * 
 * This function subscribes Timer interrupts
 * 
 * @param bit_no Address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * 
 * @return int 0 upon success, 1 otherwise
 */
int (timer_subscribe_int)(uint8_t *bit_no) {
  *bit_no = BIT(__hook_id);

  if(sys_irqsetpolicy(TIMER0_IRQ , IRQ_REENABLE , &__hook_id) != OK)
    return 1;
 
  return 0;
}

/**
 * @brief Unsubscribes Timer interrupts
 * 
 * This function unsubscribes Timer interrupts
 * 
 * @return int 0 upon success, 1 otherwise
 */
int (timer_unsubscribe_int)() {
  
  if(sys_irqrmpolicy(&__hook_id) != OK)
    return 1;
 
  return 0;
}

/**
 * @brief Timer interrupt handler
 * 
 * This function handles Timer interrupts
 */
void (timer_int_handler)() {
  counter++;
}

/**
 * @brief Timer get configuration
 * 
 * This function gets the configuration of the Timer
 * 
 * @param timer Timer to get the configuration
 * @param st Address of memory to be initialized with the configuration
 * 
 * @return int 0 upon success, 1 otherwise
 */
int (timer_get_conf)(uint8_t timer, uint8_t *st) {

  int port;

  uint8_t rb_command = TIMER_RB_CMD;

  rb_command |= TIMER_RB_COUNT_;
  rb_command |= TIMER_RB_SEL(timer);

  if(sys_outb(TIMER_CTRL , rb_command) != OK)
    return 1;

  port = TIMER_0 + timer;

  if(util_sys_inb(port , st) != OK)
    return 1;

  return 0;
}

/**
 * @brief Timer display configuration
 * 
 * This function displays the configuration of the Timer
 * 
 * @param timer Timer to display the configuration
 * @param st Configuration to display
 * @param field Field to display
 * 
 * @return int 0 upon success, 1 otherwise
 */
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
