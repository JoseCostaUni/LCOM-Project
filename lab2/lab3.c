#include <lcom/lcf.h>
#include <lcom/lab2.h>

#include <stdbool.h>
#include <stdint.h>

extern uint32_t counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/g1/lab2/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/g1/lab2/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(timer_test_read_config)(uint8_t timer, enum timer_status_field field) {


  return 0;
}

int(timer_test_time_base)(uint8_t timer, uint32_t freq) {
  
 
  return 0;
}

int(timer_test_int)(uint8_t time) {
  int ipc_status,r;
  message msg;

  uint8_t irq_set;

  if(timer_subscribe_int(&irq_set) != 0){
    return 1;
  }

  while(time != 0) { 
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
              case HARDWARE:			  
                if (msg.m_notify.interrupts & BIT(irq_set)) { //Perguntar o porquê só funcionar com o cast do BIT()
                  timer_int_handler(); //Perguntar se existe um forma diferente de passar o valor   do counter do handler;

                  if(counter % 60 == 0){ //Como gera interrupts por default a cada 60 Hz
                    time--;
                    timer_print_elapsed_time();
                  }
                }
                break;
              default:
                  break;
            }
    } else { 
    }
 }

  if(timer_unsubscribe_int() != 0){
    return 1;
  }
  return 0;
}
