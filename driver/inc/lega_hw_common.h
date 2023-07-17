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
#ifndef _LEGA_HW_COMMON_H_
#define _LEGA_HW_COMMON_H_
#include "sns_silib.h"

#ifdef __cplusplus
extern "C"{
#endif

#define LEGA_HW_OK         0
#define LEGA_HW_ERR        -1

extern CRYS_RND_Context_t   *rndContext_ptr;
extern CRYS_RND_WorkBuff_t  *rndWorkBuff_ptr;
extern int hw_lib_init_flag;

int lega_hw_lib_init();
int lega_hw_lib_deinit();

#ifdef __cplusplus
}
#endif
#endif //_LEGA_HW_COMMON_H_