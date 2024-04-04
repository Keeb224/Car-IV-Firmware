#include "sg_rtc.hpp";

extern "C" RTC_HandleTypeDef hrtc;

//Logs the date in the form Month/Day/Year | Hour:Minute:Second:SubSecond
void rtc::get_time(void)
     {
     RTC_DateTypeDef gDate;
     RTC_TimeTypeDef gTime;
     /* Get the RTC current Time */
     HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);
     /* Get the RTC current Date */
     HAL_RTC_GetDate(&hrtc, &gDate, RTC_FORMAT_BIN);

    //Formats to Month/Day/Year | Hour:Minute:Second:SubSecond
    Logger::LogInfo("%u/%u/%u | %u:%u:%u:%u", gDate.Month, gDate.Date, gDate.Year,gTime.Hours, gTime.Minutes,gTime.Seconds,gTime.SubSeconds);
     }

//Sets the date using Month,Day,Year Format
void rtc::set_date(unsigned int userMonth,unsigned int userDate, unsigned int userYear){
    RTC_DateTypeDef sDate;
       sDate.Month = convert_to_rtc_time(userMonth); // month
       sDate.Date = convert_to_rtc_time(userDate); // date
       sDate.Year = convert_to_rtc_time(userYear); //year 
       if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
       {
         Logger::LogInfo("RTC Date Not Set");
       }
        HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x32F2); // backup register
}

//Sets the time using Hours,Minuts,Seconds Format
//Input years as the year after 2000 ei.2024 enter 24
void rtc::set_time (unsigned int userHours, unsigned int userMinutes,unsigned int userSeconds)  
     {
       RTC_TimeTypeDef sTime;
       sTime.Hours = convert_to_rtc_time(userHours); // set decimal Hours to the Hex Hours
       sTime.Minutes = convert_to_rtc_time(userMinutes); // set minutes
       sTime.Seconds = convert_to_rtc_time(userSeconds); // set seconds
       sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
       sTime.StoreOperation = RTC_STOREOPERATION_RESET;
       if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
       {
         Logger::LogInfo("RTC Time Not Set");
       }
       HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x32F2); // backup register
     }

//Sets both Date and Time using Hours,Minuts,Seconds,Month,Day,Year Format
//Input year as the year after 2000 ei.2024 enter 24
void rtc::set_Date_Time(unsigned int userHours, unsigned int userMinutes,unsigned int userSeconds, unsigned int userMonth,unsigned int userDate, unsigned int userYear)
    {
        set_time(userHours,userMinutes,userSeconds);
        set_date(userMonth,userDate,userYear);
    }