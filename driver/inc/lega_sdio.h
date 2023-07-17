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
#ifndef LEGA_SDIO_H
#define LEGA_SDIO_H

#ifdef __cplusplus
extern "C"{
#endif

typedef void (*sdio_cb_hdr)(uint8_t*, uint32_t);

//init sdio hw and init sdio task
int sdio_task_init(void);
void sdio_task_deinit(void);

void sdio_register_msg_callback(sdio_cb_hdr host_tx_msg_hdr);
void sdio_deregister_msg_callback(void);

void sdio_register_data_callback(sdio_cb_hdr host_tx_data_hdr);
void sdio_deregister_data_callback(void);

//host rx data triger callback handle
void sdio_register_hrx_triger_callback(sdio_cb_hdr host_rx_triger_hdr);
void sdio_deregister_hrx_triger_callback(void);

//user tx data to sdio host
int sdio_host_rx_data_hdr(uint8_t* data, uint32_t data_len);

//user tx msg to sdio host
int sdio_host_rx_msg_hdr(uint8_t* data, uint32_t data_len);

#ifdef __cplusplus
}
#endif
#endif
