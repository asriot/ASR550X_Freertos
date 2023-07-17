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
#ifndef __RF_SPI_H__
#define __RF_SPI_H__

#ifdef __cplusplus
extern "C"{
#endif

#define RC_SW_CTRL_ADDR             0x60C0C004
#define SPI_COMMAND                 RC_SW_CTRL_ADDR
#define SPI_RDATA                   RC_SW_CTRL_ADDR

void spi_mst_write(uint16_t addr, uint16_t data );
uint16_t spi_mst_read(uint16_t addr);

#ifdef __cplusplus
}
#endif
#endif //__RF_SPI_H__