/*
 * sg_rtc.hpp
 *
 *  Created on: April 3, 2024
 *      Author: Rob Morrison
 */

#pragma once

#include "user.hpp"
#include "logger.hpp"
extern "C" RTC_HandleTypeDef hrtc;

class rtc {
public:
    rtc() = default;
    void get_Time();
    void set_Date_Time(unsigned int userHours, unsigned int userMinutes,unsigned int userSeconds, unsigned int userMonth,unsigned int userDate, unsigned int userYear);
    void set_time(unsigned int userHours, unsigned int userMinutes,unsigned int userSeconds);
    void set_date(unsigned int userMonth,unsigned int userDate, unsigned int userYear);
   
private:
    static inline unsigned int convert_to_rtc_time(unsigned int value);
};