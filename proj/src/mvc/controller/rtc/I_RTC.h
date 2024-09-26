/**
 * @file I_RTC.h
 * @brief File containing the constants and functions to interact with the RTC
*/

#ifndef _I_RTC_H_
#define _I_RTC_H_

#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include <stdint.h>


#define INP_REG 0x70
#define OUT_REG 0x71 
#define UPDATE_REG 0x0A

#define UPDATE_IN_PROGRESS BIT(7)

#define RTC_IRQ 8

#define SEC                    0x00
#define SECONDS_ALARM          0x01
#define MINUTES                0x02
#define RTC_MINUTES_ALARM      0x03
#define HOURS                  0x04
#define RTC_HOURS_ALARM        0x05
#define DAY_OF_WEEK            0x06
#define DAY_OF_MONTH           0x07
#define MONTH                  0x08
#define YEAR                   0x09
              
#define REG_A                  0x0A
#define REG_B                  0x0B
#define REG_C                  0x0C
#define REG_D                  0x0D

#endif
