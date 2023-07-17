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
#include <sys/stat.h>
#include <reent.h>
#include "lega_rtos_api.h"
#include "lega_cm4.h"
#include "lega_peripheral_reg.h"

void *_malloc_r(struct _reent *ptr, size_t size)
{
    void *mem;

    mem = lega_rtos_malloc(size);

    return mem;
}

void *_realloc_r(struct _reent *ptr, void *old, size_t newlen)
{
    return 0;
}

void *_calloc_r(struct _reent *ptr, size_t size, size_t len)
{
    void *mem;

    mem = lega_rtos_malloc(size * len);

    return mem;
}

void _free_r(struct _reent *ptr, void *addr)
{
    if(addr == NULL)
    {
        return;
    }

    lega_rtos_free(addr);
}
/////////////////////////////////////
_ssize_t _write_r(struct _reent *ptr, int fd, const void *buf, size_t nbytes)
{
    const char *tmp = buf;
    int         i   = 0;

    if (buf == NULL)
    {
        return 0;
    }

    if ((fd == 1) || (fd == 2))
    {
        for (i = 0; i < nbytes; i++)
        {
            uart_put_char(*tmp);
            tmp++;
        }
        return nbytes;
    }
    else
    {
        return -1;
    }
}
 ///////////////////////////////////
 int _isatty (int fd)
 {
     return 1;
 }
 /////////////////////////////////
 int _lseek(int i, int k, int j)
 {
      return -1;
 }
 ////////////////////////////////
 int _read (int fd, char *pBuffer, int size)
 {
     return size;
 }
 ////////////////////////////////////
 int _fstat (int fd, struct stat *pStat)
 {
      pStat->st_mode = S_IFCHR;
      return 0;
 }
 ////////////////////////////////////
 int _close(int i)
 {
      return -1;
 }

const char __locale_ctype_ptr[10];

#ifdef OS_NOT_SUPPORT
#include <stdlib.h>
#include "lega_rtos_api.h"
void *lega_rtos_malloc(uint32_t xWantedSize)
{
    return malloc(xWantedSize);
}

void lega_rtos_free(void *pv)
{
    free(pv);
}

OSStatus lega_rtos_lock_mutex( lega_mutex_t* mutex, uint32_t timeout_ms )
{
    return kNoErr;
}

OSStatus lega_rtos_unlock_mutex(lega_mutex_t* mutex)
{
    return kNoErr;
}

OSStatus lega_rtos_init_mutex(lega_mutex_t* mutex)
{
    return kNoErr;
}

OSStatus lega_rtos_deinit_mutex(lega_mutex_t* mutex)
{
    return kNoErr;
}
#endif //OS_NOT_SUPPORT
 /************end *************************/
