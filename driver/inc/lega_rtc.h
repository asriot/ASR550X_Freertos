/*
 * Copyright (c) 2022 ASR Microelectronics (Shanghai) Co., Ltd. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef _LEGA_RTC_H_
#define _LEGA_RTC_H_
#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif

#define HAL_RTC_FORMAT_DEC  1
#define HAL_RTC_FORMAT_BCD  2

typedef struct {
    uint8_t  format;    /* time formart DEC or BCD */
} lega_rtc_config_t;

typedef struct {
    uint8_t port;        /* rtc port */
    lega_rtc_config_t config; /* rtc config */
    void   *priv;        /* priv data */
} lega_rtc_dev_t;

/*
 * RTC time
 */
typedef struct {
    uint8_t sec;         /* DEC format:value range from 0 to 59 */
    uint8_t min;         /* DEC format:value range from 0 to 59 */
    uint8_t hr;          /* DEC format:value range from 0 to 23 */
    uint8_t weekday;     /* DEC format:value range from 0 to  6 */
    uint8_t date;        /* DEC format:value range from 1 to 31 */
    uint8_t month;       /* DEC format:value range from 0 to 11, 0:january  11:december*/
    uint8_t year;        /* DEC format:value range from 0 to 255, since 1900 eg:year=120 means 1900+120=2020 */
} lega_rtc_time_t;

/**
 * This function will initialize the on board CPU real time clock
 *
 *
 * @param[in]  rtc  rtc device
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t lega_rtc_init(lega_rtc_dev_t *rtc);

/**
 * This function will return the value of time read from the on board CPU real time clock.
 *
 * @param[in]   rtc   rtc device
 * @param[out]  time  pointer to a time structure
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t lega_rtc_get_time(lega_rtc_dev_t *rtc, lega_rtc_time_t *time);

/**
 * This function will set MCU RTC time to a new value.
 *
 * @param[in]   rtc   rtc device
 * @param[out]  time  pointer to a time structure
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t lega_rtc_set_time(lega_rtc_dev_t *rtc, const lega_rtc_time_t *time);

/**
 * De-initialises an RTC interface, Turns off an RTC hardware interface
 *
 * @param[in]  RTC  the interface which should be de-initialised
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t lega_rtc_finalize(lega_rtc_dev_t *rtc);

#ifdef __cplusplus
}
#endif
#endif //_LEGA_RTC_H_