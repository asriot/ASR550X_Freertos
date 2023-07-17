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
#include <stdio.h>
#include "lega_cm4.h"
#include "soc_init.h"
#include "lega_wdg.h"
#include "lega_boot.h"
#include "lega_flash.h"
#include "lega_common.h"
#include "systick_delay.h"
#include "lega_wlan_api.h"
#include "lega_flash_alg.h"
#include "FreeRTOSConfig.h"
#include "lega_version.h"

#if (configUSE_TICKLESS_IDLE == 1)
#include "pmu.h"
extern pmu_state_t current_state;
#endif

#define SYS_APP_VERSION_SEG __attribute__((section("app_version_sec")))
SYS_APP_VERSION_SEG const char app_version[] =  LEGA_VERSION_STR;

#define REG_AHB_BUS_CTRL   *((volatile uint32_t *)(ALWAYS_ON_REGFILE + 0x90))

/***********************************************************
* set IRQ priority and enable IRQ it later
*
**********************************************************/
void NVIC_init()
{
    //set irq priority, default set configLIBRARY_NORMAL_INTERRUPT_PRIORITY
    NVIC_SetPriority(UART0_IRQn,configLIBRARY_NORMAL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(UART1_IRQn,configLIBRARY_NORMAL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(UART2_IRQn,configLIBRARY_NORMAL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(ASRW_WI_IP_IRQn,configLIBRARY_NORMAL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(D_APLL_UNLOCK_IRQn,configLIBRARY_NORMAL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(D_SX_UNLOCK_IRQn,configLIBRARY_NORMAL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(SLEEP_IRQn,configLIBRARY_NORMAL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(WDG_IRQn,configLIBRARY_NORMAL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(FLASH_IRQn,configLIBRARY_NORMAL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(GPIO_IRQn,configLIBRARY_NORMAL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(TIMER_IRQn,configLIBRARY_NORMAL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(CRYPTOCELL310_IRQn,configLIBRARY_NORMAL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(DMA_CTRL_IRQn,configLIBRARY_NORMAL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(SPI0_IRQn,configLIBRARY_NORMAL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(SPI1_IRQn,configLIBRARY_NORMAL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(SPI2_IRQn,configLIBRARY_NORMAL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(I2C0_IRQn,configLIBRARY_NORMAL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(I2C1_IRQn,configLIBRARY_NORMAL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(SDIO_IRQn,configLIBRARY_NORMAL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(PLF_WAKEUP_IRQn,configLIBRARY_NORMAL_INTERRUPT_PRIORITY);
}

/***********************************************************
* watchdog init config
*
**********************************************************/
lega_wdg_dev_t lega_wdg;
void wdg_init(void)
{
    lega_wdg.port = 0;
    lega_wdg.config.timeout = WDG_TIMEOUT_MS;
    lega_wdg_init(&lega_wdg);
}

extern void lega_cca_adjust_enable(void);
/***********************************************************
*  init device: flash config,irq priority,rco and systick
*
**********************************************************/
#ifdef CFG_XTAL_40M
#define SYSTEM_CLK_XTAL40M_LOW   (40000000)
uint32_t system_clk_t    = SYSTEM_CLK_XTAL40M_LOW;
extern uint32_t system_clk_t;
#endif

#ifdef HIGHFREQ_MCU160_SUPPORT
uint32_t system_bus_clk  = SYSTEM_BUS_CLOCK_INIT;
uint32_t system_core_clk = SYSTEM_CORE_CLOCK_INIT;
#endif
extern uint8_t lega_get_chip_type(void);
extern uint8_t lega_chip_is_smic(void);
void lega_devInit()
{
    lega_get_chip_type();

#ifdef _SPI_FLASH_ENABLE_
    lega_flash_init();
#endif

    if(lega_chip_is_smic())
    {
        RTC_REG_RCO32K |= (1<<(5+16)) | (1<<(7+16)) | (1<<(8+16));
    }

    //default close watchdog for step debug on keil
    //wdg_init();

    NVIC_init();

#ifdef QUICK_CONNECT
    if (lega_get_boot_type() != DEEP_SLEEP_RST)
#endif
    lega_drv_rco_cal();

#ifdef HIGHFREQ_MCU160_SUPPORT
    delay_init(SYSTEM_CORE_CLOCK/1000000);
#else
    //Config SysTick and init delay config with api of delay_us & delay_ms
    delay_init(SYSTEM_CLOCK/1000000);
#endif

#ifndef QUICK_CONNECT
#ifndef LIGHT_SLEEP_SUPPORT
    //lega_cca_adjust_enable();
#endif
#endif

#if (configUSE_TICKLESS_IDLE == 1 && defined(LIGHT_SLEEP_SUPPORT))
    lega_pmu_init();
#endif
}

extern void lega_reset_rw_rf(void);
/**********************************************************
*  use soc_init to init board
*
**********************************************************/
int soc_init(void)
{
    lega_drv_open_dcdc_pfm();
    lega_reset_rw_rf();

    lega_devInit();
    return 0;
}


void HCLK_SW_IRQHandler(void)
{
    SYS_CRM_CLR_HCLK_REC = 0x1;
}

void delay_nop(unsigned int dly)
{
    volatile unsigned int i;
    for(i=dly; i>0; i--)
    {
    }
}
void soc_delay_us(uint32_t nus)
{
    delay_us(nus);
}
void ahb_sync_brid_open(void)
{
    unsigned int is_using_sync_down = (REG_AHB_BUS_CTRL & (0x1<<1));
    if(!is_using_sync_down || lega_get_boot_type() == DEEP_SLEEP_RST)
    {
        REG_AHB_BUS_CTRL |= (0x1<<1); //0x40000A90 bit1 sw_use_hsync

        __enable_irq();
        NVIC_EnableIRQ(24);
        __asm volatile("DSB");
        __asm volatile("WFI");
        __asm volatile("ISB");

        delay_nop(50);
    }
}

#if (configUSE_TICKLESS_IDLE == 1)
void enable_sleep_irq_after_deepsleep(void)
{
    if (lega_get_boot_type() == DEEP_SLEEP_RST)
    {
        // In this case means wakeup from deep sleep and need process RTC interrupt
        current_state = PMU_STATE_DEEPSLEEP;
        NVIC_ClearPendingIRQ(SLEEP_IRQn);
        NVIC_EnableIRQ(SLEEP_IRQn);
    }
}
#endif

#ifdef CFG_XTAL_40M
void lega_cpu_run_40m(void)
{
    // Enable: 160MHz clock divide from 480MHz. | Enable core clock divide by 2 -- But *NOT* select clk_core_div2
    SYS_CRM_SYS_CLK_CTRL1 |= SYS_CRM_SYS_CLK_CTRL1 | (0x1<<7) | (0x1<<10);
    delay(5);
    //use uart2_frac_divider[0] to enable clk_core duty-cycle=25%
    SYS_CRM_UART2_FRAC_DIV = 0x0;//0x1;
    delay(5);
    // 0x40000804 [2] clk_core_div2_sel = 1'b1 -- select core clock divided by 2
    SYS_CRM_REG_0x804 |= SYS_CRM_REG_0x804 | (0x1<<2);
    delay(5);
    // 0x40000804 [1:0] clk_core_sel = 2'b00 -- select XTAL 52/80MHz clock as core clock
    SYS_CRM_REG_0x804 = SYS_CRM_REG_0x804 & (0xFFFFFFFC);
    delay(5);
    system_clk_t      = 40000000;
}
#endif
/*************************************************************
*  soc init config, don't run any code before it
*
**************************************************************/
int soc_pre_init(void)
{
#ifdef CFG_XTAL_40M
    lega_cpu_run_40m();
#endif
    ahb_sync_brid_open();
    lega_flash_alg_cache_flush();

    // enable sleep irq here to clear interrupt status after deepsleep reset
#if (configUSE_TICKLESS_IDLE == 1)
    enable_sleep_irq_after_deepsleep();
#endif

    return 0;
}
