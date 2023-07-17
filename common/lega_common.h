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
#ifndef _LEGA_COMM_H_
#define _LEGA_COMM_H_

#ifdef __cplusplus
extern "C"{
#endif

#define LEGA_STR_TO_INT_ERR  0xFFFFFFFF

#define FLASH_COMMON2_SEG
#define FLASH_COMMON_SEG

void jumpToApp(int addr);
void lega_memset(char *buf,int value, int size);
void lega_memcpy(char *dst, char *src, int size);
void udelay(unsigned int us);
void delay(unsigned int cycles);
int convert_str_to_int(char *str);
void convert_int_to_str(unsigned int val, unsigned int type, char *ch);

/// Macro to read a register
#define REG_RD(addr)              (*(volatile uint32_t *)(addr))
/// Macro to write a register
#define REG_WR(addr, value)       (*(volatile uint32_t *)(addr)) = (value)

void lega_write32_bit(uint32_t reg, uint8_t start_bit, uint8_t len, uint32_t src_val);
uint32_t lega_read32_bit(uint32_t reg, uint8_t start_bit, uint8_t len);

#ifdef __cplusplus
}
#endif
#endif //_LEGA_COMM_H_
