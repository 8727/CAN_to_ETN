#ifndef _SETTING_H
#define _SETTING_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "Ee24cxx.h"
#include "rtc.h"
#include "rs485.h"
#include "can.h"


/* Define --------------------------------------------------------------------*/
#define VERSION_BUILD           0x302E3033  // "0.03"
#define STATUS_EEPROM           0x44        // 0x44 -default "D", 
#define DEVICE_NUMBER           0x00        // Device number
#define DEVICE_TYPE             0x05        // Device type  0x07 = Core, 0x06 = Water, 0x05 = Power/Timer start,0x04 = SW
#define RTC_CALIBRATION         0x00        // RTC CalibrationPpm
#define NAME                    "1"
#define LCD_ROTATION            0x09        // 0x27 Rotation_270, 0x18 Rotation_180, 0x09 Rotation_90, 0x00 Rotation_0
#define TIMER_START_FRIDGE      0x0380      // 14 min 56 sec
#define TIMER_START_CONDITIONER 0x0700      // 29 min 52 sec
#define TIMER_START_POWER       0x0170      //  6 min  8 sec
#define TIMER_START_RELAY_4     0x0000      // 1092 min 15sec
//#define TIMER_START_FRIDGE      0x0025
//#define TIMER_START_CONDITIONER 0x0020
//#define TIMER_START_POWER       0x0015
//#define TIMER_START_RELAY_4     0x0010
#define CALIBRATION_ENERGY      0x0000
#define ALARM_POWER             0x0898      // 2200 W


#define TIMER_PAUSE_LCD         0x05
#define TIMER_GUI_SWICH         0x0F
#define EEPROM_BUFF             0x40

/* Define --------------------------------------------------------------------*/
#define CanModeNormal           0x00
#define CanModeLoopBack         CAN1->BTR |= CAN_BTR_LBKM
#define CanModeSilent           CAN1->BTR |= CAN_BTR_SILM
#define CanModeSilentLoopBack   CAN1->BTR |= (CAN_BTR_SILM | CAN_BTR_LBKM)

#define CAN_SPEED               0x001C0004  //0x00050004 // 1000 Kb
                                            //0x001C0004 // 500 Kb
                                            //0x001C0009 // 250 Kb
                                            //0x001C0013 // 125 Kb
                                            //0x001C0018 // 100 Kb

#define RS485_SPEED             0x208D      //0x0057       //921600
                                            //0x00A0       //500000
                                            //0x00AE       //460800
                                            //0x0140       //250000
                                            //0x015B       //230400
                                            //0x0000       //125000
                                            //0x02B6       //115200
                                            //0x056D       //57600
                                            //0x0823       //38400
                                            //0x208D       //9600

#define USART_2_3_SPEED_1152    0x015B      //0x002B       //921600
#define USART_2_3_SPEED_96      0x1047      //0x0050       //500000
                                            //0x0057       //460800
                                            //0x00A0       //250000
                                            //0x00AE       //230400
                                            //0x0000       //125000
                                            //0x015B       //115200
                                            //0x02B6       //57600
                                            //0x0412       //38400
                                            //0x0823       //19200
                                            //0x1047       //9600

/* Define --------------------------------------------------------------------*/
#define ADDR_EEPROM_VERSION_BUILD               0x00 // 0x00-0x03
#define ADDR_EEPROM_DATE_BUILD                  0x04 // 0x04-0x07
#define ADDR_EEPROM_STATUS_EEPROM               0x08
#define ADDR_EEPROM_DEVICE_NUMBER               0x09
#define ADDR_EEPROM_DEVICE_TYPE                 0x0A
#define ADDR_EEPROM_RTC_CALIBRATION             0x0B
#define ADDR_EEPROM_CAN_SPEED                   0x0C // 0x0C-0x0F
#define ADDR_EEPROM_NAME                        0x10 // 0x10-0x17
#define ADDR_EEPROM_RS485_SPEED                 0x18 // 0x18-0x19
#define ADDR_EEPROM_LCD_ROTATION                0x1A
#define ADDR_EEPROM________X                    0x1B // 0x1B-0x1C
#define ADDR_EEPROM_LED_RGB                     0x1D
#define ADDR_EEPROM_LED_TOP                     0x1E
#define ADDR_EEPROM_LED_BUTTON                  0x1F
#define ADDR_EEPROM_CALIBRATION_SENSOR_1        0x20
#define ADDR_EEPROM_CALIBRATION_SENSOR_2        0x21
#define ADDR_EEPROM_CALIBRATION_SENSOR_3        0x22
#define ADDR_EEPROM_CALIBRATION_SENSOR_4        0x23
#define ADDR_EEPROM_ALARM_SENSOR_1              0x24
#define ADDR_EEPROM_ALARM_SENSOR_2              0x25
#define ADDR_EEPROM_ALARM_SENSOR_3              0x26
#define ADDR_EEPROM_ALARM_SENSOR_4              0x27
#define ADDR_EEPROM_SENSOR_WATER_ON_OFF         0x28
#define ADDR_EEPROM_CALIBRATION_PRESSURE_HOT    0x29
#define ADDR_EEPROM_CALIBRATION_PRESSURE_COLD   0x2A
#define ADDR_EEPROM_CALIBRATION_POWER_V         0x2B
#define ADDR_EEPROM_DRIFT_WARM_FLOOR            0x2C
#define ADDR_EEPROM_________X                   0x2D // 0x2D-0x2F
#define ADDR_EEPROM_TIMER_START_FRIDGE          0x30 // 0x30-0x31
#define ADDR_EEPROM_TIMER_START_CONDITIONER     0x32 // 0x32-0x33
#define ADDR_EEPROM_TIMER_START_POWER           0x34 // 0x34-0x35
#define ADDR_EEPROM_TIMER_START_RELAY_4         0x36 // 0x36-0x37
#define ADDR_EEPROM_CALIBRATION_ENERGY          0x38 // 0x38-0x3B
#define ADDR_EEPROM_ALARM_POWER                 0x3C // 0x3C-0x3D
#define ADDR_EEPROM_________X                   0x2D // 0x3E-0x3F

/* Define --------------------------------------------------------------------*/
#define PRIORITY_DHT22_DMA      0x0F
#define PRIORITY_RTC            0x0D
#define PRIORITY_RS485          0x01
#define PRIORITY_GUI_UPDATE     0x00
#define PRIORITY_DS18B20        0x00
#define PRIORITY_DHT22_TIM      0x00
#define PRIORITY_CAN_TX         0x00
#define PRIORITY_CAN_RX0        0x00
#define PRIORITY_CAN_RX1        0x00
#define PRIORITY_CAN_ERROR      0x00

/* Define --------------------------------------------------------------------*/
#define BUILD_YEAR (__DATE__[7] == '?' ? 1900 : (((__DATE__[7] - '0') * 1000 ) + (__DATE__[8] - '0') * 100 + (__DATE__[9] - '0') * 10 + __DATE__[10] - '0'))
#define BUILD_MONTH (__DATE__ [2] == '?' ? 1 : __DATE__ [2] == 'n' ? (__DATE__ [1] == 'a' ? 1 : 6) : __DATE__ [2] == 'b' ? 2 : __DATE__ [2] == 'r' ? (__DATE__ [0] == 'M' ? 3 : 4) \
        : __DATE__ [2] == 'y' ? 5 : __DATE__ [2] == 'l' ? 7 : __DATE__ [2] == 'g' ? 8 : __DATE__ [2] == 'p' ? 9 : __DATE__ [2] == 't' ? 10 : __DATE__ [2] == 'v' ? 11 : 12)
#define BUILD_DAY (__DATE__[4] == '?' ? 1 : ((__DATE__[4] == ' ' ? 0 : ((__DATE__[4] - '0') * 10)) + __DATE__[5] - '0'))
#define BUILD_TIME_H (__TIME__[0] == '?' ? 1 : ((__TIME__[0] == ' ' ? 0 : ((__TIME__[0] - '0') * 10)) + __TIME__[1] - '0'))
#define BUILD_TIME_M (__TIME__[3] == '?' ? 1 : ((__TIME__[3] - '0') * 10 + __TIME__[4] - '0'))
#define BUILD_TIME_S (__TIME__[6] == '?' ? 1 : ((__TIME__[6] - '0') * 10 + __TIME__[7] - '0'))

struct settingsInitTypeDef{
  uint8_t  type;
  uint8_t  number;
  uint32_t dateBuild;
  uint16_t canDevice;
  uint8_t  rtcCalibration;
  uint32_t canSpeed;
  uint16_t rs485Speed;
  uint8_t  rotation;
  
  uint8_t  start;
  uint8_t  swich;
  uint8_t  guiSwich;
};

struct powerInitTypeDef{
  uint32_t activeEnergy;
  uint32_t activePower;
  uint16_t voltage;
  uint32_t current;
  uint8_t  powerFctor;
  uint16_t frequency;
  uint16_t alarm;
  uint32_t calibrationEnergy;
  uint16_t alarmPower;
};

extern struct settingsInitTypeDef settings;
extern struct powerInitTypeDef power;

uint32_t GetTick(void);
void DelayMs(uint32_t ms);

void Setting(void);

#endif /* _SETTING_H */
