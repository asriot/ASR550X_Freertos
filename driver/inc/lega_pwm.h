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
#ifndef _LEGA_PWM_H_
#define _LEGA_PWM_H_
#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif

#define PWM_OUTPUT_CH0 0
#define PWM_OUTPUT_CH1 1
#define PWM_OUTPUT_CH2 2
#define PWM_OUTPUT_CH3 3
#define PWM_OUTPUT_CH4 4
#define PWM_OUTPUT_CH5 5
#define PWM_OUTPUT_CH6 6
#define PWM_OUTPUT_CH7 7
#define LEGA_PWM_CH_NUM 8

typedef struct {
    float    duty_cycle;  /* the pwm duty_cycle */
    uint32_t freq;        /* the pwm freq */
} lega_pwm_config_t;

typedef struct {
    uint8_t      port;    /* pwm port */
    lega_pwm_config_t config;  /* spi config */
    void        *priv;    /* priv data */
} lega_pwm_dev_t;

/**
 * Initialises a PWM pin
 *
 *
 * @param[in]  pwm  the PWM device
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t lega_pwm_init(lega_pwm_dev_t *pwm);

/**
 * Starts Pulse-Width Modulation signal output on a PWM pin
 *
 * @param[in]  pwm  the PWM device
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t lega_pwm_start(lega_pwm_dev_t *pwm);

/**
 * Stops output on a PWM pin
 *
 * @param[in]  pwm  the PWM device
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t lega_pwm_stop(lega_pwm_dev_t *pwm);

/**
 * change the para of pwm
 *
 * @param[in]  pwm  the PWM device
 * @param[in]  para the para of pwm
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t lega_pwm_para_chg(lega_pwm_dev_t *pwm, lega_pwm_config_t para);

/**
 * De-initialises an PWM interface, Turns off an PWM hardware interface
 *
 * @param[in]  pwm  the interface which should be de-initialised
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t lega_pwm_finalize(lega_pwm_dev_t *pwm);

#ifdef __cplusplus
}
#endif
#endif //_LEGA_PWM_H_