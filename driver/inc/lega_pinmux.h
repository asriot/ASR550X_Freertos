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
#ifndef __LEGA_PINMUX_H
#define __LEGA_PINMUX_H

#include "lega_cm4.h"

#ifdef __cplusplus
extern "C"{
#endif

#define PAD_PE_REG      0x40000014
#define PAD_PS_REG      0x40000018
#define PAD_IS_REG      0x4000001C
#define HW_CTRL_PE_PS   0x40000020
#define PINMUX_REG_BASE 0x40000004
typedef enum
{
    PAD0 = 0x00,
    PAD1,
    PAD2,
    PAD3,
    PAD4,
    PAD5,
    PAD6,
    PAD7,
    PAD8,
    PAD9,
    PAD10,
    PAD11,
    PAD12,
    PAD13,
    PAD14,
    PAD15,
}Pad_Num_Type;


//typedef enum
//{
//    PF_PWM,   // 4
//    PF_GPIO,   // 5
//    PF_SPI,        // 2
//    PF_I2C0,
//    PF_I2C1,
//    PF_UART2, // 1 or 2
//    PF_UART0, // 1 or 5
//    PF_UART1, // 1
//    PF_UART3,  // 1 or 5
//    PF_CTS_RTS,
//    PF_SWD
//}Pad_Func_Type;

typedef enum
{
    PULL_DEFAULT = 0x00,
    PULL_UP,
    PULL_DOWN,
    PULL_NONE
}Pad_Pull_Type;

typedef enum
{
    DS1DS0_00 = 0x00,
    DS1DS0_01,
    DS1DS0_10,
    DS1DS0_11
}Pad_DS_Type;

/**
 * pad function set
 *
 * @param[in]  pad_num     the pad num to set
 * @param[in]  pad_func    pad function num 
 *
 * @return  NULL
 */
void lega_pinmux_config(Pad_Num_Type pad_num, uint8_t pad_func);
/**
 * pad pull type set
 *
 * @param[in]  pad_num     the pad num to set
 * @param[in]  pull_type   pull type select
 *
 * @return  NULL
 */
void lega_pad_config(Pad_Num_Type pad_num, Pad_Pull_Type pull_type);

#ifdef __cplusplus
}
#endif
#endif /*__LEGA_PINMUX_H */
