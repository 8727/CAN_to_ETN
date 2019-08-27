#include "rtc.h"

/*
01) [UTC - 12]    Baker Island Time
02) [UTC - 11]    Niue Time, Samoa Standard Time
03) [UTC - 10]    Hawaii-Aleutian Standard Time, Cook Island Time
04) [UTC - 9:30]  Marquesas Islands Time
05) [UTC - 9]     Alaska Standard Time, Gambier Island Time
06) [UTC - 8]     Pacific Standard Time
07) [UTC - 7]     Mountain Standard Time
08) [UTC - 6]     Central Standard Time
09) [UTC - 5]     Eastern Standard Time
10) [UTC - 4:30]  Venezuelan Standard Time
11) [UTC - 4]     Atlantic Standard Time
12) [UTC - 3:30]  Newfoundland Standard Time
13) [UTC - 3]     Amazon Standard Time, Central Greenland Time
14) [UTC - 2]     Fernando de Noronha Time, South Georgia &amp; the South Sandwich Islands Time
15) [UTC - 1]     Azores Standard Time, Cape Verde Time, Eastern Greenland Time
16) [UTC]         Western European Time, Greenwich Mean Time
17) [UTC + 1]     Central European Time, West African Time
18) [UTC + 2]     Eastern European Time, Central African Time
19) [UTC + 3]     Moscow Standard Time, Eastern African Time
20) [UTC + 3:30]  Iran Standard Time
21) [UTC + 4]     Gulf Standard Time, Samara Standard Time
22) [UTC + 4:30]  Afghanistan Time
23) [UTC + 5]     Pakistan Standard Time, Yekaterinburg Standard Time
24) [UTC + 5:30]  Indian Standard Time, Sri Lanka Time
25) [UTC + 5:45]  Nepal Time
26) [UTC + 6]     Bangladesh Time, Bhutan Time, Novosibirsk Standard Time
27) [UTC + 6:30]  Cocos Islands Time, Myanmar Time
28) [UTC + 7]     Indochina Time, Krasnoyarsk Standard Time
290 [UTC + 8]     Chinese Standard Time, Australian Western Standard Time, Irkutsk Standard Time
30) [UTC + 8:45]  Southeastern Western Australia Standard Time
31) [UTC + 9]     Japan Standard Time, Korea Standard Time, Chita Standard Time
32) [UTC + 9:30]  Australian Central Standard Time
33) [UTC + 10]    Australian Eastern Standard Time, Vladivostok Standard Time
34) [UTC + 10:30] Lord Howe Standard Time
34) [UTC + 11]    Solomon Island Time, Magadan Standard Time
36) [UTC + 11:30] Norfolk Island Time
37) [UTC + 12]    New Zealand Time, Fiji Time, Kamchatka Standard Time
38) [UTC + 12:45] Chatham Islands Time
39) [UTC + 13]    Tonga Time, Phoenix Islands Time
40) [UTC + 14]    Line Island Time
*/

void RTC_IRQHandler(void){
  if(RTC->CRL & RTC_CRL_SECF){
    RTC->CRL &= ~RTC_CRL_SECF;
    
    #if defined DEBUG_RTC
      RtcTimeStamp();
      printf("\r\n");
    #endif
    if(0x00 != settings.dhcpOn && 0x00 != settings.dhcpSt) EthernetDhcpRutine();
  }
}

uint16_t RtcTimeZoneToSeconds(uint8_t time_zone){
  uint16_t seconds;
  seconds = ZONE_SEC[time_zone - 0x01];
  return seconds;
}

uint32_t RtcTimeZoneAdjustment(uint32_t time, uint8_t time_zone){
  uint16_t adjustment;
  adjustment = RtcTimeZoneToSeconds(time_zone);
  if(0x10 > time_zone){ time -= adjustment; }else{ time += adjustment; }
  return time;
}
 
void RtcTimeStamp(void){
  RtcTypeDef date;
  RtcSecondsToTime(RtcGetSeconds(), &date);
  printf("\t%d.%d.%d %d:%02d:%02d\t", date.day, date.month, date.year, date.hour, date.min, date.sec);
}

void RtcSecondsToTime(uint32_t seconds, RtcTypeDef* unixTime){
  uint32_t a, t;
  char b, c, d;
  t = seconds % SEC_A_DAY;
  a = ((seconds + 43200) / (SEC_A_DAY >> 1)) + (2440587 << 1) + 1;
  a >>= 1;
  unixTime->wday = a % 7;
  a += 32044;
  b = (4 * a + 3) / 146097;
  a = a - (146097 * b) / 4;
  c = (4 * a + 3) / 1461;
  a = a - (1461 * c) / 4;
  d = (5 * a + 2) / 153;
  unixTime->day = a - (153 * d + 2) / 5 + 1;
  unixTime->month = d + 3 - 12 * (d / 10);
  unixTime->year = 100 * b + c - 4800 + (d / 10);
  unixTime->hour = t / 3600;
  unixTime->min = (t % 3600) / 60;
  unixTime->sec = (t % 3600) % 60;
}

uint32_t RtcTimeToSeconds(RtcTypeDef* unixTime){
  uint8_t a, m;
  uint16_t y;
  a = ((14 - unixTime->month) / 12);
  y = unixTime->year + 4800 - a;
  m = unixTime->month + (12 * a) - 3;
  return (((unixTime->day + ((153 * m + 2) / 5) + 365 * y + (y / 4) - (y / 100) + (y / 400) - 32045) - 2440588) * 
    SEC_A_DAY) + unixTime->sec + unixTime->min * 60 + unixTime->hour * 3600;
}

uint32_t RtcGetSeconds(void){
  return (uint32_t)((RTC->CNTH << 0x10) | RTC->CNTL);
}

void RtcSetSeconds(uint32_t seconds){
  PWR->CR |= PWR_CR_DBP;
  while(!(RTC->CRL & RTC_CRL_RTOFF));
  RTC->CRL |= RTC_CRL_CNF;
  RTC->CNTH = seconds >> 0x10;
  RTC->CNTL = seconds;
  RTC->CRL &= ~RTC_CRL_CNF;
  while(!(RTC->CRL & RTC_CRL_RTOFF));
  PWR->CR &= ~PWR_CR_DBP;
}

void RtcInit(void){
  if((RCC->BDCR & RCC_BDCR_RTCEN) != RCC_BDCR_RTCEN){
//    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
//    RCC->APB1ENR |= RCC_APB1ENR_BKPEN;
    PWR->CR |= PWR_CR_DBP;
    RCC->BDCR |= RCC_BDCR_BDRST;
    RCC->BDCR &= ~RCC_BDCR_BDRST;
    RCC->BDCR |= RCC_BDCR_RTCEN;
    RCC->BDCR |= RCC_BDCR_RTCSEL_LSE;
    RCC->BDCR |= RCC_BDCR_LSEON;
    while((RCC->BDCR & RCC_BDCR_LSEON) != RCC_BDCR_LSEON){}
    BKP->RTCCR |= settings.rtcCalib;
    while(!(RTC->CRL & RTC_CRL_RTOFF));
    RTC->CRL |= RTC_CRL_CNF;
    RTC->PRLL = 0x7FFF;
//    BKP->RTCCR |= BKP_RTCCR_CCO;
    RTC->CNTH = settings.dateBuild >> 0x10;
    RTC->CNTL = settings.dateBuild;
    RTC->CRL &= ~RTC_CRL_CNF;
    while(!(RTC->CRL & RTC_CRL_RTOFF));
    RTC->CRL &= (uint16_t) ~RTC_CRL_RSF;
    while((RTC->CRL & RTC_CRL_RSF) != RTC_CRL_RSF){}
    PWR->CR &= ~PWR_CR_DBP;
  }
  RTC->CRH |= RTC_CRH_SECIE;
//  RTC->CRH |= RTC_CRH_ALRIE;
//  RTC->CRH |= RTC_CRH_OWIE;
  NVIC_SetPriority(RTC_IRQn, PRIORITY_RTC);
  NVIC_EnableIRQ(RTC_IRQn);
  
  #if defined DEBUG_RTC
    printf("< OK >    Initialization RTC");
    RtcTimeStamp();
    printf("\r\n");
  #endif
}
