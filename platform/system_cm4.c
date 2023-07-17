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
#include "lega_cm4.h"
#include "core_cm4.h"
#ifdef HIGHFREQ_MCU160_SUPPORT
#include "lega_rf.h"
#endif

uint8_t g_chip_is_smic = 0;

#ifdef CFG_RESET_OPT
extern void lega_drv_open_dcdc_pfm(void);
extern void lega_reset_rw_rf(void);
extern void lega_clk_sel_xtal(void);
extern void lega_drv_move_rf_to_lo(void);
#endif

void lega_system_reset(void)
{
    //disable irq when reboot
    __disable_irq();

#ifdef HIGHFREQ_MCU160_SUPPORT
    if(system_core_clk == SYSTEM_CORE_CLOCK_HIGH)
        lega_clk_sel_low();
#endif

#ifdef CFG_RESET_OPT
    lega_clk_sel_xtal();

    lega_drv_move_rf_to_lo();
    lega_drv_open_dcdc_pfm();
    lega_reset_rw_rf();
#endif

    NVIC_SystemReset();
}

#ifdef QUICK_CONNECT
extern void         *flash_start;
extern void         *ram_data_start;
extern void         *ram_data_end;
extern void         *ram_bss_start;
extern void         *ram_bss_end;
__attribute__((section("seg_critical_function"))) void SystemInit(void)
#else
void SystemInit(void)
#endif
{
  /* FPU settings ------------------------------------------------------------*/
  #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */
  #endif

#ifdef QUICK_CONNECT
  uint32_t * p_flash_start = (uint32_t *)&flash_start;
  uint32_t * p_ram_data_start = (uint32_t *)&ram_data_start;
  uint32_t * p_ram_data_end = (uint32_t *)&ram_data_end;
  uint32_t len = 0;
  while (p_ram_data_start + len < p_ram_data_end)
  {
      *(p_ram_data_start + len) = *(p_flash_start + len);
      len++;
  }

  uint32_t * p_ram_bss_start = (uint32_t *)&ram_bss_start;
  uint32_t * p_ram_bss_end = (uint32_t *)&ram_bss_end;
  while (p_ram_bss_start < p_ram_bss_end)
  {
     *(p_ram_bss_start) = 0;
     p_ram_bss_start++;
  }
#endif
}


void SystemCoreClockUpdate(void)
{

}




/********END OF FILE ***********/
