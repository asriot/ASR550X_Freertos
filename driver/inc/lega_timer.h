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
#ifndef _LEGA_TIMER_H_
#define _LEGA_TIMER_H_
#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif

#define LEGA_TIMER1_INDEX 0
#define LEGA_TIMER2_INDEX 1
#define LEGA_TIMER_NUM 2

#define TIMER_RELOAD_AUTO  1  /* timer reload automatic */
#define TIMER_RELOAD_MANU  2  /* timer reload manual */

typedef void (*lega_timer_cb_handler_t)(void *arg);
typedef struct {
    lega_timer_cb_handler_t cb;
    void *arg;
} lega_timer_cb_t;

typedef struct {
    uint32_t       period;   /*us*/
    uint8_t        reload_mode;
    lega_timer_cb_handler_t cb;
    void          *arg;
} lega_timer_config_t;

typedef struct {
    int8_t         port;   /* timer port */
    lega_timer_config_t config; /* timer config */
    void          *priv;   /* priv data */
} lega_timer_dev_t;

/**
 * init a hardware timer
 *
 * @param[in]  tim  timer device
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t lega_timer_init(lega_timer_dev_t *tim);

/**
 * start a hardware timer
 *
 * @param[in]  tim  timer device
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t lega_timer_start(lega_timer_dev_t *tim);

/**
 * get hardware timer remain time
 *
 * @return   success return remain time, EIO == failure
 */
int32_t lega_timer_get(lega_timer_dev_t *tim);

/**
 * reload hardware timer value
 *
 * @return   0 == success, EIO == failure
 */
int32_t lega_timer_reload(lega_timer_dev_t *tim);

/**
 * stop a hardware timer
 *
 * @param[in]  tim  timer device
 *
 * @return  none
 */
void lega_timer_stop(lega_timer_dev_t *tim);

/**
 * De-initialises an TIMER interface, Turns off an TIMER hardware interface
 *
 * @param[in]  tim  timer device
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t lega_timer_finalize(lega_timer_dev_t *tim);

#ifdef __cplusplus
}
#endif
#endif //_LEGA_TIMER_H_