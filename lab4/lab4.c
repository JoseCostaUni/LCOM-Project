// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include <lcom/lab4.h>
#include <lcom/timer.h>
#include <kbc.h>
#include <i8042.h>
#include <mouse.h>
#include <i8254.h>

extern struct packet mouse;
extern int counter;
int counter_packet_print = 0;
extern int counter_timer;
extern enum States current_state;


// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

/*
  configure the mouse properly - no idea how to do this
  receive the number of mouse packets specified in its argument via interrupts  mouse_ih()
  parse the mouse packets 
  print them in a friendly way (mouse_print_packet())
  reset the mouse to Minix's default configuration, before exiting sync_bytes()
*/
int (mouse_test_packet)(uint32_t cnt) {
     /* To be completed */
    printf("%s: under construction\n", __func__);
    return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
    


    return 1;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
    /* To be completed */
    printf("%s: under construction\n", __func__);
    return 1;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* This year you need not implement this. */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}
