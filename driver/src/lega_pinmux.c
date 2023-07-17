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
#include "lega_pinmux.h"

void lega_pinmux_config(Pad_Num_Type pad_num, uint8_t pad_func)
{
    uint32_t pad_reg_addr = PINMUX_REG_BASE + 4*(pad_num>>3); // pinmux register address of pad pad_num
    uint32_t reg_offset = (pad_num%8)*4;  // offset from pad_reg_addr in bits
    uint32_t pad_func_value = pad_func;

    *(volatile uint32_t *)(pad_reg_addr) &= ~(0xf<<reg_offset); // mask
    *(volatile uint32_t *)(pad_reg_addr) |= (pad_func_value<<reg_offset); // set pinmux value
}

void lega_pad_config(Pad_Num_Type pad_num, Pad_Pull_Type pull_type)
{
    /* assert params */
    // to-do
     switch(pull_type)
     {
         case PULL_UP:
             *(volatile uint32_t *)(HW_CTRL_PE_PS) &= ~(0x1 << (pad_num));
             *(volatile uint32_t *)(PAD_PE_REG) |= (0x1 << (pad_num));
             *(volatile uint32_t *)(PAD_PS_REG) |= (0x1 << (pad_num));
             break;
         case PULL_DOWN:
             *(volatile uint32_t *)(HW_CTRL_PE_PS) &= ~(0x1 << (pad_num));
             *(volatile uint32_t *)(PAD_PE_REG) |= (0x1 << (pad_num));
             *(volatile uint32_t *)(PAD_PS_REG) &= ~(0x1 << (pad_num));
             break;
         case PULL_NONE:
             *(volatile uint32_t *)(HW_CTRL_PE_PS) &= ~(0x1 << (pad_num));
             *(volatile uint32_t *)(PAD_PE_REG) &= ~(0x1 << (pad_num));
             break;
         default:
             break;
     }
}
