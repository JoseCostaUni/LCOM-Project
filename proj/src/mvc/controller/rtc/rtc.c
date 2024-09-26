/**
 * @file rtc.c
 * @brief File containing the implementation of the functions to interact with the RTC
*/

#include "rtc.h"

int __hook_id_rtc = 6;
/**
 * @brief Subscribes and enables RTC interrupts
 * 
 * This function subscribes and enables interrupts from the RTC.
 * 
 * @param bit_no Address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * @return int 0 upon success, -1 otherwise
*/
int (rtc_subscribe_int)(uint8_t *bit_no){
	*bit_no = __hook_id_rtc;
	if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE, &__hook_id_rtc) != 0){
		printf("Error subscribing rtc\n");
		return -1;
	}
	return 0;
}

/**
 * @brief Unsubscribes RTC interrupts
 * 
 * This function unsubscribes RTC interrupts.
 * 
 * @return int 0 upon success, -1 otherwise
*/
int (rtc_unsubscribe_int)() {
	if (sys_irqrmpolicy(&__hook_id_rtc) != 0){
		printf("Error unsubscribing rtc\n");
		return -1;
	}
	return 0;
}


/**
 * @brief Reads a register from the RTC
 * 
 * This function reads a register from the RTC.
 * 
 * @param reg Register to be read
 * @param val Address of memory to be initialized with the value read from the register
 * @return int 0 upon success, -1 otherwise
*/
int (read_register) (uint32_t reg, uint8_t* val){
	
	if (sys_outb(INP_REG, reg) != 0) 
		return -1;
	
	if (util_sys_inb(OUT_REG, val) != 0) 
		return -1;

	return 0;
}

/**
 * @brief Checks if an update is in progress
 * 
 * This function checks if an update is in progress.
 * 
 * @return 1 if an update is in progress, 0 otherwise, -1 upon failure
*/
int (update_in_progress) (){
	uint8_t st;

	if (read_register(UPDATE_REG, &st) != 0) 
		return -1;

	if(st & UPDATE_IN_PROGRESS)
		return 1;

	return 0;
}

/**
 * @brief Converts a BCD value to decimal
 * 
 * This function converts a BCD value to decimal.
 * 
 * @param value Address of memory to be initialized with the decimal value
*/
void (convert_bcd_to_decimal) (uint8_t* value){
	uint8_t decimal = (*value & 0xF0) >> 4;
	uint8_t unity = *value & 0x0F;
	*value = decimal * 10 + unity;
}

/**
 * @brief Converts a decimal value to BCD
 * 
 * This function converts a decimal value to BCD.
 * 
 * @param value Address of memory to be initialized with the BCD value
*/
void (convert_decimal_to_bcd) (uint8_t* value){
	uint8_t first_hex = (*value / 10) << 4;
	uint8_t last_hex = *value % 10;
	*value = first_hex | last_hex;
}

/**
 * @brief Adjusts the values of a date
 * 
 * This function adjusts the values of a date.
 * 
 * @param date Address of memory to be initialized with the adjusted date
*/
void (adjust_date_values) (current_date* date){
	convert_bcd_to_decimal(&date->seconds);
	convert_bcd_to_decimal(&date->minutes);
	convert_bcd_to_decimal(&date->hours);
	convert_bcd_to_decimal(&date->day);
	convert_bcd_to_decimal(&date->day_week);
	convert_bcd_to_decimal(&date->month);
	convert_bcd_to_decimal(&date->year);
}


/**
 * @brief Gets the current date
 * 
 * This function gets the current date.
 * 
 * @param date Address of memory to be initialized with the current date
 * @return int 0 upon success, -1 otherwise
*/
int (rtc_get_current_date) (current_date* date){
	
	while (true){
		
		int ret = update_in_progress();

		if(ret == -1){
			return -1;
		}
		else if(ret == 1){
			break;
		}
	}

	if (read_register(SEC, &date->seconds) != 0) return -1;

	if (read_register(MINUTES, &date->minutes) != 0) return -1;

	if (read_register(HOURS, &date->hours) != 0) return -1;

	if (read_register(DAY_OF_MONTH, &date->day) != 0) return -1;

	if (read_register(DAY_OF_WEEK, &date->day_week) != 0) return -1;

	if (read_register(MONTH, &date->month) != 0) return -1;

	if (read_register(YEAR, &date->year) != 0) return -1;

	adjust_date_values(date);
	return 0;
}



/**
 * @brief Prints the date
 * 
 * This function prints the date.
 * 
 * @param date Address of memory with the date to be printed
*/
void (print_date) (current_date* date) {
    printf("%04d-%02d-%02d", 2000 + date->year, date->month, date->day);
}


/**
 * @brief Prints the time
 * 
 * This function prints the time.
 * 
 * @param date Address of memory with the time to be printed
*/
void (print_time) (current_date* date) {
    printf("%02d:%02d:%02d", date->hours, date->minutes, date->seconds);
}
