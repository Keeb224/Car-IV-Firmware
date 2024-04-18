#include "user.hpp"
#include "/Users/robmorrisn/Development/Solar-Gators/Car-IV-Firmware/Resources/ubxlib/ubxlib.h"

/* In this program, two tasks are generated. One is triggered by a periodic timer, which
 * expires every 500ms. The other is a regular thread, which is triggered by and event flag.
 * The periodic timer task sets the event flag, which causes the regular thread to exit
 * its wait state and execute. The regular thread then toggles the LED and prints a message.
 * 
 * CPP_UserSetup is called from main.cpp, and is where the user should put their setup code.
 * It is run before the RTOS scheduler is started.
*/

extern "C" void CPP_UserSetup(void);
extern "C" RTC_HandleTypeDef hrtc;

/* Task function prototypes */
void PeriodicTask1(void *argument);
void RegularTask1(void *argument);
void set_time(unsigned int userHours, unsigned int userMinutes,unsigned int userSeconds);
void set_date(unsigned int userMonth,unsigned int userDate, unsigned int userYear);

/* Periodic timer definitions */
osTimerAttr_t periodic_timer_attr = {
    .name = "Periodic Task 1",
    .attr_bits = 0,
    .cb_mem = NULL,
    .cb_size = 0,
};
osTimerId_t periodic_timer_id = osTimerNew((osThreadFunc_t)PeriodicTask1, osTimerPeriodic, NULL, &periodic_timer_attr);

/* Regular task definitions */
osThreadId_t regular_task_id;
uint32_t regular_task_buffer[128];
StaticTask_t regular_task_control_block;
const osThreadAttr_t regular_task_attributes = {
    .name = "Regular Task 1",
    .attr_bits = osThreadDetached,
    .cb_mem = &regular_task_control_block,
    .cb_size = sizeof(regular_task_control_block),
    .stack_mem = &regular_task_buffer[0],
    .stack_size = sizeof(regular_task_buffer),
    .priority = (osPriority_t) osPriorityAboveNormal,
    .tz_module = 0,
    .reserved = 0,
};

/* Event flag to trigger regular task */
osEventFlagsId_t regular_event = osEventFlagsNew(NULL);


void CPP_UserSetup(void) {
    // Make sure that timer priorities are configured correctly
    HAL_Delay(10);

    regular_task_id = osThreadNew((osThreadFunc_t)RegularTask1, NULL, &regular_task_attributes);
    osTimerStart(periodic_timer_id, 1000);

}

void PeriodicTask1(void *argument) {
    Logger::LogInfo("Periodic timer fired\n");
    osEventFlagsSet(regular_event, 0x1);
}


//Logs the date in the form Month/Day/Year | Hour:Minute:Second:SubSecond
void get_time(void)
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

    //The RTC takes input in a weird form of HEX
    //Converts decimal to this form of hex
    //Ex: 11 --> 0x11
    static inline unsigned int convert_to_rtc_time(unsigned int value) {
        return value / 10 * 16 + value % 10;
    }

    void set_Date_Time(unsigned int userHours, unsigned int userMinutes,unsigned int userSeconds, unsigned int userMonth,unsigned int userDate, unsigned int userYear)
    {
        set_time(userHours,userMinutes,userSeconds);
        set_date(userMonth,userDate,userYear);
    }

    void set_time (unsigned int userHours, unsigned int userMinutes,unsigned int userSeconds)  
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

    void set_date(unsigned int userMonth,unsigned int userDate, unsigned int userYear)
    {
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

void RegularTask1(void *argument) {

    while (1) {
        osEventFlagsWait(regular_event, 0x1, osFlagsWaitAny, osWaitForever);
        get_time();
        Logger::LogInfo("Hello World!\n");
        HAL_GPIO_TogglePin(OK_LED_GPIO_Port, OK_LED_Pin);
    }
}

void getCoordinates(){

}