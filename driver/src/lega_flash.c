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
#include <string.h>
#include "lega_cm4.h"
#include "lega_flash_alg.h"
#include "lega_flash.h"
#ifndef OS_NOT_SUPPORT
#ifdef ALIOS_SUPPORT
#include "lega_rhino.h"
#else
#include "lega_rtos_api.h"
#endif
#endif
#define FLASH_ACCESS_CONTROL0_DEFAULT_VALUE 0x37053977
#define FLASH_ACCESS_CONTROL1_DEFAULT_VALUE 0x000014e5
extern const lega_logic_partition_t lega_partitions[];

#define WRITE_FLAG 1
#define ERASE_FLAG 0
#define ROUND_DOWN(x, align) ((unsigned long)(x) & ~((unsigned long)align - 1))

static lega_partition_t flash_layout_judge(uint32_t addr)
{
    lega_partition_t out_partition;
    if(addr < BOOTLOADER_FLASH_START_ADDR)
        out_partition = PARTITION_ERROR;
    else if(addr >= BOOTLOADER_FLASH_START_ADDR && addr < BOOTLOADER_FLASH_START_ADDR + BOOTLOADER_MAX_SIZE)
        out_partition = PARTITION_BOOTLOADER;
    else if(addr >= INFO_FLASH_START_ADDR && addr < INFO_FLASH_START_ADDR + INFO_MAX_SIZE)
        out_partition = PARTITION_PARAMETER_1;
    else if(addr >= APP_FLASH_START_ADDR && addr < APP_FLASH_START_ADDR + APP_MAX_SIZE)
        out_partition = PARTITION_APPLICATION;
    else if(addr >= OTA_FLASH_START_ADDR && addr < OTA_FLASH_START_ADDR + OTA_MAX_SIZE)
        out_partition = PARTITION_OTA_TEMP;
    else if(addr >= KV_FLASH_START_ADDR && addr < KV_FLASH_START_ADDR + KV_MAX_SIZE)
        out_partition = PARTITION_PARAMETER_2;
    else
        out_partition = PARTITION_ERROR;
    return out_partition;
}

static int32_t refresh_flash_sector(lega_partition_t current_partition, uint32_t sector_offset, uint32_t start_offset, unsigned char *buf, uint32_t len, uint32_t flag)
{
    int32_t ret = 0;
    lega_partition_t tran_partition;
#ifdef OS_NOT_SUPPORT
    char tmpbuf[SPI_FLASH_SEC_SIZE] = {0};
#else
    char *tmpbuf = (char *)lega_rtos_malloc((uint32_t)SPI_FLASH_SEC_SIZE);
#endif
    char *pend = NULL;
    uint32_t sector_start = sector_offset;
    uint32_t end_offset = start_offset + len;
    memset(tmpbuf, 0, SPI_FLASH_SEC_SIZE);
    //read
    if((FLASH_ACCESS_CONTROL0_DEFAULT_VALUE == FLASH_ACCESS_CONTROL0) && (FLASH_ACCESS_CONTROL1_DEFAULT_VALUE == FLASH_ACCESS_CONTROL1) ) //default flash remapping
    {
        tran_partition = current_partition;
    }
    else
    {
        if(current_partition == PARTITION_OTA_TEMP)
            tran_partition = PARTITION_APPLICATION;
        else if(current_partition == PARTITION_APPLICATION)
            tran_partition = PARTITION_OTA_TEMP;
        else
            tran_partition = current_partition;
    }
    ret = lega_flash_read(tran_partition, &sector_start, tmpbuf, SPI_FLASH_SEC_SIZE);
    if(ret != 0)
    {
        printf("flash_read error\n");
        #ifndef OS_NOT_SUPPORT
        lega_rtos_free(tmpbuf);
        #endif
        return ret;
    }
    //erase
    sector_start = sector_offset;
    ret = lega_flash_erase(current_partition, sector_start, SPI_FLASH_SEC_SIZE);
    if(ret != 0)
    {
        printf("flash_erase error\n");
        #ifndef OS_NOT_SUPPORT
        lega_rtos_free(tmpbuf);
        #endif
        return ret;
    }
    //write
    if(flag == WRITE_FLAG)
    {
        memcpy(tmpbuf + start_offset - sector_offset, buf, len);
        sector_start = sector_offset;
        ret = lega_flash_write(current_partition, &sector_start, tmpbuf, SPI_FLASH_SEC_SIZE);
        if(ret != 0)
        {
            printf("flash_write error\n");
            #ifndef OS_NOT_SUPPORT
            lega_rtos_free(tmpbuf);
            #endif
            return ret;
        }
    }
    else if(flag == ERASE_FLAG)
    {
        if(sector_offset < start_offset)
        {
            sector_start = sector_offset;
            ret = lega_flash_write(current_partition, &sector_start, tmpbuf, start_offset - sector_offset);
            if(ret != 0)
            {
                printf("flash_write error\n");
                #ifndef OS_NOT_SUPPORT
                lega_rtos_free(tmpbuf);
                #endif
                return ret;
            }
        }
        if(end_offset - sector_offset < SPI_FLASH_SEC_SIZE)
        {
            pend = tmpbuf + start_offset - sector_offset + len;
            ret = lega_flash_write(current_partition, &end_offset, pend, SPI_FLASH_SEC_SIZE - (end_offset - sector_offset));
            if(ret != 0)
            {
                printf("flash_write error\n");
                #ifndef OS_NOT_SUPPORT
                lega_rtos_free(tmpbuf);
                #endif
                return ret;
            }
        }
    }
#ifndef OS_NOT_SUPPORT
    lega_rtos_free(tmpbuf);
#endif
    return ret;
}
/**
 * lega flash init
 *
 * @note this function must be called before flash erase/write operation
 *       also, this function must not be interrupted, it should be called
 *       with interrupt disabled
 * @param[in]  none
 *
 * @return     HAL_logi_partition struct
 */
int32_t lega_flash_init(void)
{
    return lega_flash_alg_init();
}
/**
 * Get the infomation of the specified flash area
 *
 * @param[in]  in_partition  The target flash logical partition
 *
 * @return     HAL_logi_partition struct
 */
lega_logic_partition_t *lega_flash_get_info(lega_partition_t in_partition)
{
    lega_logic_partition_t *logic_partition;

    logic_partition = (lega_logic_partition_t *)&lega_partitions[ in_partition ];

    return logic_partition;
}

/**
 * Erase an area on a Flash logical partition
 *
 * @note  Erase on an address will erase all data on a sector that the
 *        address is belonged to, this function does not save data that
 *        beyond the address area but in the affected sector, the data
 *        will be lost.
 *        this function must not be interrupted, it should be called with
 *        interrupt disabled
 *
 * @param[in]  in_partition  The target flash logical partition which should be erased
 * @param[in]  off_set       Start address of the erased flash area
 * @param[in]  size          Size of the erased flash area
 *
 * @return  0 : On success, EIO : If an error occurred with any step
 */
int32_t lega_flash_erase(lega_partition_t in_partition, uint32_t off_set, uint32_t size)
{
    uint32_t addr;
    uint32_t start_addr, end_addr;
    int32_t ret = 0;
    lega_logic_partition_t *partition_info;

    partition_info = lega_flash_get_info( in_partition );
    if(size + off_set > partition_info->partition_length)
    {
        return -1;
    }

    start_addr = ROUND_DOWN((partition_info->partition_start_addr + off_set), SPI_FLASH_SEC_SIZE);
    end_addr = ROUND_DOWN((partition_info->partition_start_addr + off_set + size - 1), SPI_FLASH_SEC_SIZE);

    size = end_addr - start_addr + SPI_FLASH_SEC_SIZE;
    addr = start_addr;
    while(size)
    {
        if(!(addr % SPI_FLASH_BLOCK_64K_SIZE) && (size > (SPI_FLASH_BLOCK_64K_SIZE-SPI_FLASH_SEC_SIZE)))
        {
            ret = lega_flash_alg_erase(BLOCK64_ERASE_CMD, addr); //250ms
            if (ret != 0)
            {
                return ret;
            }
            addr += SPI_FLASH_BLOCK_64K_SIZE;
            if(size > SPI_FLASH_BLOCK_64K_SIZE)
            {
                size -= SPI_FLASH_BLOCK_64K_SIZE;
            }
            else
            {
                size = 0;
            }
        }
        else if(!(addr % SPI_FLASH_BLOCK_32K_SIZE) && (size > (SPI_FLASH_BLOCK_32K_SIZE-SPI_FLASH_SEC_SIZE)))
        {
            ret = lega_flash_alg_erase(BLOCK32_ERASE_CMD, addr); //170ms
            if (ret != 0)
            {
                return ret;
            }
            addr += SPI_FLASH_BLOCK_32K_SIZE;
            if(size > SPI_FLASH_BLOCK_32K_SIZE)
            {
                size -= SPI_FLASH_BLOCK_32K_SIZE;
            }
            else
            {
                size = 0;
            }
        }
        else
        {
            ret = lega_flash_alg_erase(SECTOR_ERASE_CMD, addr); //100ms
            if (ret != 0)
            {
                return ret;
            }
            addr += SPI_FLASH_SEC_SIZE;
            if(size > SPI_FLASH_SEC_SIZE)
            {
                size -= SPI_FLASH_SEC_SIZE;
            }
            else
            {
                size = 0;
            }
        }
    }

    return 0;
}



/**
 * Write data to an area on a flash logical partition without erase
 *
 * @note this function must not be interrupted, it should be called with
 *       interrupt disabled
 *
 * @param[in]  in_partition    The target flash logical partition which should be read which should be written
 * @param[in]  off_set         Point to the start address that the data is written to, and
 *                             point to the last unwritten address after this function is
 *                             returned, so you can call this function serval times without
 *                             update this start address.
 * @param[in]  inBuffer        point to the data buffer that will be written to flash
 * @param[in]  inBufferLength  The length of the buffer
 *
 * @return  0 : On success, EIO : If an error occurred with any step
 */
int32_t lega_flash_write(lega_partition_t in_partition, uint32_t *off_set,
                        const void *in_buf, uint32_t in_buf_len)
{
    uint32_t start_addr, end_addr;
    lega_logic_partition_t *partition_info;
    uint8_t *p_buf = (uint8_t *)(in_buf);
    uint32_t prg_size;
    uint32_t left_buf_len = in_buf_len;

    partition_info = lega_flash_get_info( in_partition );
    if(off_set == NULL || in_buf == NULL || ((*off_set + in_buf_len) > partition_info->partition_length))
    {
        // ptr and size over range check
        return -1;
    }
    start_addr = partition_info->partition_start_addr + *off_set;
    end_addr = start_addr + left_buf_len;

    while(left_buf_len > 0)
    {
        if((end_addr / SPI_FLASH_PAGE_SIZE) > (start_addr / SPI_FLASH_PAGE_SIZE))
        {
            prg_size = SPI_FLASH_PAGE_SIZE - (start_addr % SPI_FLASH_PAGE_SIZE);
        }
        else
        {
            prg_size = left_buf_len;
        }
        lega_flash_alg_programpage(start_addr, prg_size, p_buf);
        p_buf += prg_size;
        start_addr += prg_size;
        left_buf_len -= prg_size;
    }

    *off_set += in_buf_len;
    return 0;
}

/**
 * Write data to an area on a flash logical partition with erase first
 *
 * @note this function must not be interrupted, it should be called with
 *       interrupt disabled
 *
 * @param[in]  in_partition    The target flash logical partition which should be read which should be written
 * @param[in]  off_set         Point to the start address that the data is written to, and
 *                             point to the last unwritten address after this function is
 *                             returned, so you can call this function serval times without
 *                             update this start address.
 * @param[in]  inBuffer        point to the data buffer that will be written to flash
 * @param[in]  inBufferLength  The length of the buffer
 *
 * @return  0 : On success, EIO : If an error occurred with any step
 */
int32_t lega_flash_erase_write(lega_partition_t in_partition, uint32_t *off_set,
                              const void *in_buf, uint32_t in_buf_len)
{
    int32_t ret = 0;
    ret = lega_flash_erase(in_partition, *off_set, in_buf_len);
    if (ret != 0)
    {
        return ret;
    }
    return lega_flash_write(in_partition, off_set, in_buf, in_buf_len);
}

/**
 * Read data from an area on a Flash to data buffer in RAM
 *
 * @param[in]  in_partition    The target flash logical partition which should be read
 * @param[in]  off_set         Point to the start address that the data is read, and
 *                             point to the last unread address after this function is
 *                             returned, so you can call this function serval times without
 *                             update this start address.
 * @param[in]  outBuffer       Point to the data buffer that stores the data read from flash
 * @param[in]  inBufferLength  The length of the buffer
 *
 * @return  0 : On success, EIO : If an error occurred with any step
 */
int32_t lega_flash_read(lega_partition_t in_partition, uint32_t *off_set,
                       void *out_buf, uint32_t in_buf_len)
{
    int32_t ret = 0;
    uint32_t start_addr;
    lega_logic_partition_t *partition_info;

    partition_info = lega_flash_get_info( in_partition );

    if(off_set == NULL || out_buf == NULL || *off_set + in_buf_len > partition_info->partition_length)
    {
        return -1;
    }
    start_addr = partition_info->partition_start_addr + *off_set;
    memcpy(out_buf, (void *)(start_addr), in_buf_len);
    *off_set += in_buf_len;

    return ret;
}

/**  Erase internal flash
 *
 * This will erase the flash for the specified start address
 * and size
 *
 * \param addr Start address in flash (*MUST* 4 KB alignment)
 * \param len Number of bytes to be erased (*MUST* 4 KB alignment, erase must in one partition)
 * \return 0 on success
 */
int32_t lega_flash_erase_common(uint32_t addr, uint32_t len)
{
    int32_t ret = 0;
    uint32_t off_set = 0;
    uint32_t sector_offset = 0;
    uint32_t start_offset = 0;
    uint32_t start_addr, end_addr;
    uint32_t sec_num = 0;
    int32_t i = 0;
    unsigned char * tmpbuf = NULL;
    lega_logic_partition_t *partition_info;
    lega_partition_t current_partition = flash_layout_judge(addr);
    if(current_partition == PARTITION_ERROR)
        return -1;
    partition_info = lega_flash_get_info(current_partition);
    if(addr == 0 || ((addr + len) > ( partition_info->partition_start_addr+partition_info->partition_length)) \
    ||(addr < partition_info->partition_start_addr))
    {
        // ptr and size over range check
        return -1;
    }
    off_set = addr - partition_info->partition_start_addr;
    start_addr = ROUND_DOWN((partition_info->partition_start_addr + off_set), SPI_FLASH_SEC_SIZE);
    end_addr = ROUND_DOWN((partition_info->partition_start_addr + off_set + len - 1), SPI_FLASH_SEC_SIZE);
    sec_num = (end_addr - start_addr)/SPI_FLASH_SEC_SIZE;
    if(sec_num > 0)
    {
        for(i = 0; i <= sec_num; i++)
        {
            if(i == 0)
            {
                sector_offset = start_addr - partition_info->partition_start_addr;
                start_offset = off_set;
                ret = refresh_flash_sector(current_partition, sector_offset, start_offset, tmpbuf, SPI_FLASH_SEC_SIZE - (start_offset - sector_offset), ERASE_FLAG);
                if(ret != 0)
                {
                    return ret;
                }
            }
            else if(i == sec_num)
            {
                sector_offset = start_addr - partition_info->partition_start_addr + i * SPI_FLASH_SEC_SIZE;
                start_offset = sector_offset;
                ret = refresh_flash_sector(current_partition, sector_offset, start_offset, tmpbuf, partition_info->partition_start_addr + off_set + len - end_addr, ERASE_FLAG);
                if(ret != 0)
                {
                    return ret;
                }
            }
            else
            {
                sector_offset = start_addr - partition_info->partition_start_addr + i * SPI_FLASH_SEC_SIZE;
                start_offset = sector_offset;
                ret = refresh_flash_sector(current_partition, sector_offset, start_offset, tmpbuf, SPI_FLASH_SEC_SIZE, ERASE_FLAG);
                if(ret != 0)
                {
                    return ret;
                }
            }
        }
    }
    else if(sec_num == 0)
    {
        sector_offset = start_addr - partition_info->partition_start_addr;
        ret = refresh_flash_sector(current_partition, sector_offset, off_set, tmpbuf, len, ERASE_FLAG);
        if(ret != 0)
        {
            return ret;
        }
    }
    else
    {
        ret = -1;
    }
    return ret;
}

/**  Write data to internal flash
 *
 * This will write specified number of bytes to flash.
 * Note that flash don't need to be erased before writing.
 *
 * \param addr Write address in flash
 * \param buff Pointer to the data to be written
 * \param len Length of data to be written, make sure write only in one partition
 *
 * \return 0 on success
 */
int32_t lega_flash_write_common(uint32_t addr, const void *in_buff, uint32_t len)
{
    int32_t ret = 0;
    uint32_t off_set = 0;
    uint32_t sector_offset = 0;
    uint32_t start_offset = 0;
    uint32_t start_addr, end_addr;
    uint32_t sec_num = 0;
    int32_t i = 0;
    unsigned char * tmpbuf = NULL;
    lega_logic_partition_t *partition_info;
    lega_partition_t current_partition = flash_layout_judge(addr);
    if(current_partition == PARTITION_ERROR)
        return -1;
    partition_info = lega_flash_get_info(current_partition);
    if(addr == 0 || in_buff == NULL || ((addr + len) > ( partition_info->partition_start_addr+partition_info->partition_length)) \
    ||(addr < partition_info->partition_start_addr))
    {
        // ptr and size over range check
        return -1;
    }
    off_set = addr - partition_info->partition_start_addr;
    start_addr = ROUND_DOWN((partition_info->partition_start_addr + off_set), SPI_FLASH_SEC_SIZE);
    end_addr = ROUND_DOWN((partition_info->partition_start_addr + off_set + len - 1), SPI_FLASH_SEC_SIZE);
    sec_num = (end_addr - start_addr)/SPI_FLASH_SEC_SIZE;
    if(sec_num > 0)
    {
        tmpbuf = (unsigned char *)in_buff;
        for(i = 0; i <= sec_num; i++)
        {
            if(i == 0)
            {
                sector_offset = start_addr - partition_info->partition_start_addr;
                start_offset = off_set;
                ret = refresh_flash_sector(current_partition, sector_offset, start_offset, tmpbuf, SPI_FLASH_SEC_SIZE - (start_offset - sector_offset), WRITE_FLAG);
                tmpbuf = tmpbuf + SPI_FLASH_SEC_SIZE - (start_offset - sector_offset);
                if(ret != 0)
                {
                    return ret;
                }
            }
            else if(i == sec_num)
            {
                sector_offset = start_addr - partition_info->partition_start_addr + i * SPI_FLASH_SEC_SIZE;
                start_offset = sector_offset;
                ret = refresh_flash_sector(current_partition, sector_offset, start_offset, tmpbuf, partition_info->partition_start_addr + off_set + len - end_addr, WRITE_FLAG);
                if(ret != 0)
                {
                    return ret;
                }
            }
            else
            {
                sector_offset = start_addr - partition_info->partition_start_addr + i * SPI_FLASH_SEC_SIZE;
                start_offset = sector_offset;
                ret = refresh_flash_sector(current_partition, sector_offset, start_offset, tmpbuf, SPI_FLASH_SEC_SIZE, WRITE_FLAG);
                tmpbuf = tmpbuf + SPI_FLASH_SEC_SIZE;
                if(ret != 0)
                {
                    return ret;
                }
            }
        }
    }
    else if(sec_num == 0)
    {
        sector_offset = start_addr - partition_info->partition_start_addr;
        ret = refresh_flash_sector(current_partition, sector_offset, off_set, (unsigned char *)in_buff, len, WRITE_FLAG);
        if(ret != 0)
        {
            return ret;
        }
    }
    else
    {
        ret = -1;
    }
    return ret;
}

/**
 * Set security options on a logical partition
 *
 * @param[in]  partition  The target flash logical partition
 * @param[in]  offset     Point to the start address that the data is read, and
 *                        point to the last unread address after this function is
 *                        returned, so you can call this function serval times without
 *                        update this start address.
 * @param[in]  size       Size of enabled flash area
 *
 * @return  0 : On success, EIO : If an error occurred with any step
 */
int32_t lega_flash_enable_secure(lega_partition_t partition, uint32_t off_set, uint32_t size)
{
    return 0;
}

/**
 * Disable security options on a logical partition
 *
 * @param[in]  partition  The target flash logical partition
 * @param[in]  offset     Point to the start address that the data is read, and
 *                        point to the last unread address after this function is
 *                        returned, so you can call this function serval times without
 *                        update this start address.
 * @param[in]  size       Size of disabled flash area
 *
 * @return  0 : On success, EIO : If an error occurred with any step
 */
int32_t lega_flash_dis_secure(lega_partition_t partition, uint32_t off_set, uint32_t size)
{
    return 0;
}

