/**
 * @file rtc.h
 * @brief File containing the implementation of the functions to interact with the RTC
*/

#ifndef RTC_H
#define RTC_H

#include <stdint.h>
#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include "I_RTC.h"

/**
 * @brief Struct to represent date and time.
 */
typedef struct{
    uint8_t seconds;  /**< Seconds */  
    uint8_t minutes;  /**< Minutes */
    uint8_t hours;    /**< Hours */
    uint8_t day;      /**< Day */  
    uint8_t month;    /**< Month */  
    uint8_t year;     /**< Year */      
    uint8_t day_week; /**< Day of the week */   
} current_date;


/**
 * @brief Subscribes RTC interrupts
 * 
 * @param bit_no Address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * 
 * @return int 0 upon success, -1 otherwise
*/
int (rtc_subscribe_int) (uint8_t *bit_no);


/**
 * @brief Unsubscribes RTC interrupts
 * 
 * @return int 0 upon success, -1 otherwise
*/
int (rtc_unsubscribe_int) ();

/**
 * @brief Reads a register from the RTC
 * 
 * @param reg Register to be read
 * @param st Address of memory to be initialized with the value read from the register
 * 
 * @return int 0 upon success, -1 otherwise
*/
int (read_register) (uint32_t reg, uint8_t* st);

/**
 * @brief Converts a BCD value to decimal
 * 
 * @param value Address of memory to be initialized with the decimal value
 */
void (convert_bcd_to_decimal) (uint8_t* value);

/**
 * @brief Converts a decimal value to BCD
 * 
 * @param value Address of memory to be initialized with the BCD value
 */
void (convert_decimal_to_bcd) (uint8_t* value);

/**
 * @brief Reads the current date from the RTC
 * 
 * @param dt Address of memory to be initialized with the current date
 * 
 * @return int 0 upon success, -1 otherwise
 */
int (rtc_get_current_date) (current_date* dt);

/**
 * @brief Prints the date
 * 
 * @param dt Address of memory to be initialized with the current date
 */
void (print_date) (current_date* dt);

/**
 * @brief Prints the time
 * 
 * @param dt Address of memory to be initialized with the current date
 */
void (print_time) (current_date* dt);

/**
 * @brief Adjusts the date values
 * 
 * @param dt Address of memory to be initialized with the current date
 */
void (adjust_date_values) (current_date* dt);


#endif /* RTC_H */


