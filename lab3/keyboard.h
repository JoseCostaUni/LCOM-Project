#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdbool.h>
#include <stdint.h>

struct scancode_info {
  uint8_t size_counter;
  bool make_code;
  uint8_t bytes[2];
};

int keyboard_subscribe_int(uint8_t *bit_no);
int keyboard_unsubscribe_int();
int read_status_register(uint8_t *st);
int read_out_buffer(uint8_t *out);
bool(receive_keyboard_scan)(struct scancode_info *scan_info, uint8_t *scancode);
void(clean_scan_info)(struct scancode_info *scan);
bool(test_status_polling)(uint8_t st);
int(kbc_enable_interrupts)();
int try_read_out_buffer(uint8_t *out);

#endif /* KEYBOARD_H */
