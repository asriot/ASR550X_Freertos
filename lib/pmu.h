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
#ifndef __PMU_H
#define __PMU_H

#include "stdint.h"
#ifdef ALIOS_SUPPORT
#include <k_api.h>
#else
#include "FreeRTOSConfig.h"
#define RHINO_CONFIG_CPU_PWR_MGMT (0)
#endif
#include <stdbool.h>

#ifdef __cplusplus
extern "C"{
#endif

typedef enum pmu_state
{
    PMU_STATE_ACTIVE = 0,
    PMU_STATE_MODEMSLEEP,
    PMU_STATE_LIGHTSLEEP,
    PMU_STATE_DEEPSLEEP,
#ifdef LIGHT_SLEEP_SUPPORT
    PMU_STATE_LIGHTSLEEP_SOC_OFF,
#endif
    PMU_STATE_LAST
}pmu_state_t;

/** \brief  Wait For Interrupt

    Wait For Interrupt is a hint instruction that suspends execution
    until one of a number of events occurs.
 */
__attribute__( ( always_inline ) ) static inline void __WFI__(void)
{
    __asm volatile( "dsb" );
    __asm volatile( "wfi" );
    __asm volatile( "isb" );
}

#define FLASH_PMU_SEG __attribute__((section("seg_flash_driver")))

/** @brief  get current RTC counter, only used when RTC enabled.
 *
 *  @return   : current RTC counter
 */
uint32_t lega_drv_rtc_counter_get();

/** @brief  get MAX RTC counter we can set, only used when RTC enabled.
 *
 *  @return   : MAX RTC counter we can set
 */
uint32_t lega_drv_rtc_max_ticks_get();

/** @brief  get MAX RTC counter we can set, only used when RTC enabled.
 *  @param    cc_counter: RTC counter to set.
 *  @param    whitch state you want goto, default modemsleep.
 *  @return   0: set OK.
 *  @return   other: set error.
 */
uint16_t lega_drv_rtc_cc_set(uint32_t cc_counter, pmu_state_t pmu_state);

/** @brief  enable RTC clock, used after lega_drv_rtc_cc_set.
 */
void lega_drv_rtc_enable();

/** @brief  goto sleep state.
 *  @param  pmu_state: state to set:MODEMSLEEP/LIGHTSLEEP/DEEPSLEEP.
 *  @param  pmu_state: state to set:MODEMSLEEP/LIGHTSLEEP/DEEPSLEEP.
 */
void lega_drv_goto_sleep(pmu_state_t pmu_state);

/** @brief  goto active state, call when waked from SLEEP states.
 */
void lega_drv_goto_active();

/** @brief  check if wakeup gpio is valid or not.
 */
uint8_t lega_drv_wakeup_gpio_is_valid(uint8_t wakeup_gpio);

/** @brief  goto deepsleep state.
 */
void lega_drv_goto_deepsleep(uint32_t sleep_ms, uint8_t wakeup_gpio);

#ifdef LIGHT_SLEEP_SUPPORT
#define TICK_TO_US_FOR_32K(x)       (((x)*1000+16.384)/32.768)
#define US_TO_TICK_FOR_32K(x)       (((x)*32.768+500)/1000)
#define TICK_TO_MS_FOR_32K(x)       (((x)*1000+16384)/32768)
#define MS_TO_TICK_FOR_32K(x)       (((x)*32768+500)/1000)

#define XTAL_WAIT_TO_VALUE          100

// tick from xtal ready to irq occur
#define XTAL_READY_TO_IRQ           18
#define RTC_RECOVERY_TICK_MARGIN    (XTAL_WAIT_TO_VALUE+XTAL_READY_TO_IRQ)
#define IRQ_TO_RWNX_WAKEUP_MARGIN   24
#define RWON_TO_TSF_RECOVERY_MARGIN 42
// xtal wait time + 18 ticks between xtal and LPO interrupt + irq to rwnx_wakeup finish
#define RWOFF_WAKEUP_TICK_MARGIN    (XTAL_WAIT_TO_VALUE+XTAL_READY_TO_IRQ+IRQ_TO_RWNX_WAKEUP_MARGIN)

enum
{
    WAKEUP_RWOFF_BY_INTERRUPT,
    WAKEUP_RWOFF_BY_HOST_TX,
    WAKEUP_RWOFF_BY_HOST_MSG,
    WAKEUP_RWOFF_BY_OTHER
};

struct pmu_rw_off_env_tag
{
    uint32_t machw_time_wakeup;
    uint32_t abs_timer_time;
    uint32_t lpo_timer_count;
    uint32_t rtc_timer_count;
    uint32_t bcn_interval_time;
    bool rw_status_on;
    bool hw_status_doze;
    bool rw_off_allowed;
    volatile bool in_light_sleep;
    volatile bool rw_waiting_turn_on;
};

extern volatile struct pmu_rw_off_env_tag pmu_rw_off_env;

void lega_pmu_init();
uint32_t lega_pmu_rtc_count_elapsed_get();
void lega_pmu_rtc_count_set(uint32_t rtc_count);
void lega_pmu_goto_lightsleep();
void lega_pmu_goto_active();
#if RW_OFF_ENABLE
void lega_pmu_rw_on(uint8_t wakeup_src);
void lega_pmu_rw_off();
bool lega_pmu_lpo_timer_set();
void lega_pmu_abs_timer_time_update(uint32_t time);
bool lega_pmu_sleep_allowed();
void lega_pmu_rw_off_allowed_set(bool status);
void lega_pmu_bcn_interval_set(uint32_t bcn_int);
#endif
#if NX_POWERSAVE
void lega_pmu_prevent_sleep_for_kmsg(bool sleep_allow);
#endif
#endif

#ifdef __cplusplus
}
#endif
#endif //__PMU_H





























