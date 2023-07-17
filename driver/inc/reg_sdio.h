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
#ifndef REG_SDIO_H
#define REG_SDIO_H

#ifdef CFG_SDIO_SUPPORT

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "lega_common.h"
#include "lega_pinmux.h"
#include "lega_rtos_api.h"



//========================sdio hw start==============================

#ifdef LEGA_A0V2
#define SDIO_CARD_BASE             0x40007000
#else
#define SDIO_CARD_BASE             0x40030000
#endif
#define SDIO_HOST                  (SDIO_CARD_BASE+0x100)
#define SDIO_BITMAP                (SDIO_CARD_BASE+0x104)  //4 byte
#define SDIO_RDBITMAP_L            (SDIO_CARD_BASE+0x104)
#define SDIO_RDBITMAP_U            (SDIO_CARD_BASE+0x105)
#define SDIO_WRBITMAP_L            (SDIO_CARD_BASE+0x106)
#define SDIO_WRBITMAP_U            (SDIO_CARD_BASE+0x107)

#define SDIO_RD_LEN_0_1            (SDIO_CARD_BASE+0x108)
#define SDIO_RD_LEN_0L             (SDIO_CARD_BASE+0x108)
#define SDIO_RD_LEN_0U             (SDIO_CARD_BASE+0x109)
#define SDIO_RD_LEN_1L             (SDIO_CARD_BASE+0x10A)
#define SDIO_RD_LEN_1U             (SDIO_CARD_BASE+0x10B)

#define SDIO_RD_LEN_2_3            (SDIO_CARD_BASE+0x10C)
#define SDIO_RD_LEN_2L             (SDIO_CARD_BASE+0x10C)
#define SDIO_RD_LEN_2U             (SDIO_CARD_BASE+0x10D)
#define SDIO_RD_LEN_3L             (SDIO_CARD_BASE+0x10E)
#define SDIO_RD_LEN_3U             (SDIO_CARD_BASE+0x10F)

#define SDIO_RD_LEN_4_5            (SDIO_CARD_BASE+0x110)
#define SDIO_RD_LEN_4L             (SDIO_CARD_BASE+0x110)
#define SDIO_RD_LEN_4U             (SDIO_CARD_BASE+0x111)
#define SDIO_RD_LEN_5L             (SDIO_CARD_BASE+0x112)
#define SDIO_RD_LEN_5U             (SDIO_CARD_BASE+0x113)

#define SDIO_RD_LEN_6_7            (SDIO_CARD_BASE+0x114)
#define SDIO_RD_LEN_6L             (SDIO_CARD_BASE+0x114)
#define SDIO_RD_LEN_6U             (SDIO_CARD_BASE+0x115)
#define SDIO_RD_LEN_7L             (SDIO_CARD_BASE+0x116)
#define SDIO_RD_LEN_7U             (SDIO_CARD_BASE+0x117)

#define SDIO_RD_LEN_8_9            (SDIO_CARD_BASE+0x118)
#define SDIO_RD_LEN_8L             (SDIO_CARD_BASE+0x118)
#define SDIO_RD_LEN_8U             (SDIO_CARD_BASE+0x119)
#define SDIO_RD_LEN_9L             (SDIO_CARD_BASE+0x11A)
#define SDIO_RD_LEN_9U             (SDIO_CARD_BASE+0x11B)

#define SDIO_RD_LEN_10_11          (SDIO_CARD_BASE+0x11C)
#define SDIO_RD_LEN_10L            (SDIO_CARD_BASE+0x11C)
#define SDIO_RD_LEN_10U            (SDIO_CARD_BASE+0x11D)
#define SDIO_RD_LEN_11L            (SDIO_CARD_BASE+0x11E)
#define SDIO_RD_LEN_11U            (SDIO_CARD_BASE+0x11F)

#define SDIO_RD_LEN_12_13          (SDIO_CARD_BASE+0x120)
#define SDIO_RD_LEN_12L            (SDIO_CARD_BASE+0x120)
#define SDIO_RD_LEN_12U            (SDIO_CARD_BASE+0x121)
#define SDIO_RD_LEN_13L            (SDIO_CARD_BASE+0x122)
#define SDIO_RD_LEN_13U            (SDIO_CARD_BASE+0x123)

#define SDIO_RD_LEN_14_15          (SDIO_CARD_BASE+0x124)
#define SDIO_RD_LEN_14L            (SDIO_CARD_BASE+0x124)
#define SDIO_RD_LEN_14U            (SDIO_CARD_BASE+0x125)
#define SDIO_RD_LEN_15L            (SDIO_CARD_BASE+0x126)
#define SDIO_RD_LEN_15U            (SDIO_CARD_BASE+0x127)


#define SDIO_HOST_RESTART          (SDIO_CARD_BASE+0x128)  //4 byte

#define SDIO_C2H_INTEVENT          (SDIO_CARD_BASE+0x130)  //4 byte

#define SDIO_CARD_INTMASK          (SDIO_CARD_BASE+0x134)  //4 byte

#define SDIO_CARD_INTSTATUS        (SDIO_CARD_BASE+0x138)  //4 byte
#define TX_DONE_INT                (1 << 0)
#define RX_DONE_INT                (1 << 1)
#define HOST_READ_ABORT_INT        (1 << 2)
#define POWER_DOWN_INT             (1 << 3)
#define POWER_UP_INT               (1 << 4)
#define UNDER_FLOW_INT             (1 << 5)
#define OVER_FLOW_INT              (1 << 6)
#define HOST_TO_CARD_INT           (1 << 7)
#define SDIO_CARD_INTRST           (SDIO_CARD_BASE+0x13C)  //4 byte

#define SDIO_RD_BASE               (SDIO_CARD_BASE+0x140)
#define SDIO_RD_BASE_0             (SDIO_CARD_BASE+0x140)
#define SDIO_RD_BASE_1             (SDIO_CARD_BASE+0x141)
#define SDIO_RD_BASE_2             (SDIO_CARD_BASE+0x142)
#define SDIO_RD_BASE_3             (SDIO_CARD_BASE+0x143)

#define SDIO_WR_BASE               (SDIO_CARD_BASE+0x144)
#define SDIO_WR_BASE_0             (SDIO_CARD_BASE+0x144)
#define SDIO_WR_BASE_1             (SDIO_CARD_BASE+0x145)
#define SDIO_WR_BASE_2             (SDIO_CARD_BASE+0x146)
#define SDIO_WR_BASE_3             (SDIO_CARD_BASE+0x147)

#define SDIO_RD_WR_IDX             (SDIO_CARD_BASE+0x148)
#define SDIO_RD_IDX                (SDIO_CARD_BASE+0x148)
#define SDIO_WR_IDX                (SDIO_CARD_BASE+0x149)

#define SDIO_SCRATCH               (SDIO_CARD_BASE+0x160)  //4 byte
#define SDIO_SCRATCH_0             (SDIO_CARD_BASE+0x160)
#define SDIO_SCRATCH_1             (SDIO_CARD_BASE+0x161)
#define SDIO_SCRATCH_2             (SDIO_CARD_BASE+0x162)
#define SDIO_SCRATCH_3             (SDIO_CARD_BASE+0x163)

#define SDIO_SCRATCH1_0             (SDIO_CARD_BASE+0x164)
#define SDIO_SCRATCH1_1             (SDIO_CARD_BASE+0x165)
#define SDIO_SCRATCH1_2             (SDIO_CARD_BASE+0x166)
#define SDIO_SCRATCH1_3             (SDIO_CARD_BASE+0x167)


//for jlink debug on flash boot
#define PAD_JTAG_SWC    PAD0
#define PAD_JTAG_SWD    PAD1


#define PAD_SDIO_CMD    PAD4
#define PAD_SDIO_CLK    PAD5
#define PAD_SDIO_DAT0    PAD6
#define PAD_SDIO_DAT1    PAD7
#define PAD_SDIO_DAT2    PAD8
#define PAD_SDIO_DAT3    PAD9


#define PIN_MUX_SDIO_FUNC    0x2
#define PIN_MUX_JTAG_FUNC    0x2

#define GET_BIT(pos)      (1U<<(pos))

#define SDIO_CLK_RST_REG_ADDR      0x40000940

#define SDIO_CARD_CONFIG           (SDIO_CARD_BASE+0x168)

#define SDIO_RE_ENABLE_CONFIG2     (SDIO_CARD_BASE+0x16C)  //4 byte

#define SDIO_PORT_IDX_0  0
#define SDIO_PORT_IDX_1  1
#define SDIO_PORT_IDX_2  2
#define SDIO_PORT_IDX_3  3
#define SDIO_PORT_IDX_4  4
#define SDIO_PORT_IDX_5  5
#define SDIO_PORT_IDX_6  6
#define SDIO_PORT_IDX_7  7
#define SDIO_PORT_IDX_8  8
#define SDIO_PORT_IDX_9  9
#define SDIO_PORT_IDX_10 10
#define SDIO_PORT_IDX_11 11
#define SDIO_PORT_IDX_12 12
#define SDIO_PORT_IDX_13 13
#define SDIO_PORT_IDX_14 14
#define SDIO_PORT_IDX_15 15
#define SDIO_PORT_IDX(idx) SDIO_PORT_IDX_##idx

#define SDIO_PORT_IDX_TXDATA_START  1
#define SDIO_PORT_IDX_RXDATA_START  2
#define SDIO_PORT_IDX_NUM           16

#define GPIO_TEMP0                  0x40000038
#define GPIO_TEMP0_MASK             0xff0000ff
#define GPIO_TEMP0_VAL              0x22000002
#define GPIO_TEMP1                  0x4000003c
#define GPIO_TEMP1_VAL              0x00002222

#define PIN_MUX_REG2                0x40000048
#define PIN_MUX_REG3                0x4000004c


#define CRC_SUCCESS  0x4000
#define CRC_FAIL     0x0
#define BOOT_SUCCESS 0x8000

#define SDIO_BLKSZ   32

#define hif_get_sdio_rx_bitmap() (REG_RD(SDIO_BITMAP) & 0xFFFF)
#define hif_get_sdio_tx_bitmap() ((REG_RD(SDIO_BITMAP) & 0xFFFF0000) >> 16)

#define hif_trigger_sdio_rx() (REG_WR(SDIO_C2H_INTEVENT, 0x02))
#define HIF_SDIO_TX_TRIGGER_THREAD 1
#define hif_trigger_sdio_tx() (REG_WR(SDIO_C2H_INTEVENT, 0x01))

#define H2C_INTEVENT_RD     (0x0<<7)
#define H2C_INTEVENT_WR     (0x1<<7)
#define H2C_INTEVENT_REG_IDX    (0x7A)
#define H2C_INTEVENT_REG_NUM    (0x7B)

#define RX_AGG_TRI_CNT 5
#define TX_AGG_TRI_CNT 5

//rx data delay trigger
#define RX_TRIGGER_PER_CNT

typedef struct
{
    uint32_t tx_comp_num;
    uint32_t rx_comp_num;
    uint32_t read_abort_num;
    uint32_t power_down_num;
    uint32_t power_up_num;
    uint32_t under_flow_num;
    uint32_t over_flow_num;
    uint32_t host_to_card_num;
}sdio_int_count_t;
//========================sdio hw end==============================


#define SDIO_MSG_TASK_API           100
#define TX_DATA_END_TOKEN           (0xAECDBFCA)
#define SDIO_HOST_TX_END_LEN            4
#define MAX_SDIO_TXRX_SIZE              1600
#define MAX_SDIO_HOST_TX_PAYLOAD_SIZE     (MAX_SDIO_TXRX_SIZE - sizeof(struct sdio_host_txdesc) - SDIO_HOST_TX_END_LEN)
#define MAX_SDIO_HOST_RX_PAYLOAD_SIZE     (MAX_SDIO_TXRX_SIZE - sizeof(struct sdio_host_rxdesc))

#define IPC_E2A_MSG_PARAM_SIZE      256

#define SDIO_MSG_HDR_LEN (sizeof(struct sdio_msg) )


#define TX_MSG_BUFF_CNT     (3)
#define RX_MSG_BUFF_CNT     (3)
#define TX_DATA_BUFF_CNT    (5)
#define RX_DATA_BUFF_CNT    (5)

#define CO_ALIGN32_HI(val) (((val)+31)&~0x1F)


enum{
    SDIO_MSG_RESET_REQ,
    SDIO_MSG_RESET_CFM,
    SDIO_MSG_ATCMD_REQ,
    SDIO_MSG_ATCMD_RSP,
    SDIO_MSG_MAX,
};

//----------------------sdio task-----------------------
enum
{
    SDIO_EVT_HOST_RX_TIMER,
    SDIO_EVT_HOST_TX_TIMER,
    SDIO_EVT_SDIO_RX_COMP,
    SDIO_EVT_SDIO_TX_COMP,
    SDIO_EVT_HOST_TX_DATA,
    SDIO_EVT_HOST_TX_MSG,
    SDIO_EVT_HOST_RX_DATA_TRIGER,

    SDIO_EVT_MAX
};

#define SDIO_EVT_HOST_RX_TIMER_BIT  GET_BIT(31 - SDIO_EVT_HOST_RX_TIMER)
#define SDIO_EVT_HOST_TX_TIMER_BIT  GET_BIT(31 - SDIO_EVT_HOST_TX_TIMER)
#define SDIO_EVT_SDIO_RX_COMP_BIT   GET_BIT(31 - SDIO_EVT_SDIO_RX_COMP)
#define SDIO_EVT_SDIO_TX_COMP_BIT   GET_BIT(31 - SDIO_EVT_SDIO_TX_COMP)
#define SDIO_EVT_HOST_TX_DATA_BIT   GET_BIT(31 - SDIO_EVT_HOST_TX_DATA)
#define SDIO_EVT_HOST_TX_MSG_BIT    GET_BIT(31 - SDIO_EVT_HOST_TX_MSG)
#define SDIO_EVT_HOST_RX_DATA_TRIGER_BIT    GET_BIT(31 - SDIO_EVT_HOST_RX_DATA_TRIGER)


#define SDIO_TASK_NAME                  "SDIO_task"
#define SDIO_TASK_PRIORITY              (28)
#define SDIO_TASK_STACK_SIZE            2048
#define SDIO_TASK_SEM_TIMEOUT_MS        3000

/// trigger sdio tx Interval default value
#define TRI_SDIO_TX_DEFAULT_INTV_MS     (4)
#define TRI_SDIO_RX_DEFAULT_INTV_MS     (4)
#define SDIO_TX_TIMER_DELAY_NUM         (6)
#define SDIO_RX_TIMER_DELAY_NUM         (4)


#define ASSERT_ERR(cond)                              \
    do {                                              \
        if (!(cond)) {                                \
            printf("ERROR: func=%s, line=%u", __func__, __LINE__); \
        }                                             \
    } while(0)

#define ASSERT_REC(cond)             ASSERT_ERR(cond)

static void sdio_rx_timer_hdr(int dummy);
static void sdio_tx_timer_hdr(int dummy);
//------------------------------------------------------


//------------list---------------------------------------------------
/// structure of a list element header
struct sdio_list_hdr
{
    /// Pointer to the next element in the list
    struct sdio_list_hdr *next;
};

/// structure of a list
struct sdio_list
{
    /// pointer to first element of the list
    struct sdio_list_hdr *first;
    /// pointer to the last element
    struct sdio_list_hdr *last;
};
//--------------------------------------------------------------------

typedef enum
{
    HIF_RX_MSG,
    HIF_RX_DATA,
    HIF_TX_MSG,
    HIF_TX_DATA,
}HIF_TYPE_E;

typedef struct
{
    struct sdio_list_hdr hdr;     //list header for chaining
    uint16_t           type;
    uint16_t           len;
    uint32_t           addr;
    uint32_t           time;
    uint8_t            idx;
}hif_elmt_t;
#define HIF_HDR_LEN sizeof(hif_elmt_t)

typedef struct
{
    volatile uint8_t  tx_data_cur_idx;
    volatile uint8_t  rx_data_cur_idx;
    volatile uint16_t tx_bitmap;
    volatile uint32_t tx_attatch_times;
    lega_timer_t trigger_sdio_tx_timer; // Timer used for trigger tx rdy int Interval
    lega_timer_t trigger_sdio_rx_timer; // Timer used for trigger rx rdy int Interval
    volatile uint32_t rx_attatch_times;
    volatile uint16_t rx_bitmap;
    volatile uint8_t  host_tx_data_cur_idx;
}hif_env_t;


typedef struct
{
    struct sdio_list hif_rx_msg_list;
    struct sdio_list hif_rx_data_list;
    struct sdio_list hif_rx_msg_sdio_list;
    struct sdio_list hif_rx_msg_empty_list;
    struct sdio_list hif_rx_data_sdio_list;
    struct sdio_list hif_rx_data_empty_list;
}hif_rx_list_t;

typedef struct
{
    struct sdio_list hif_tx_msg_list;
    struct sdio_list hif_tx_data_list;
    struct sdio_list hif_tx_msg_empty_list;
    struct sdio_list hif_tx_data_empty_list;
    struct sdio_list hif_tx_msg_sdio_list;
    struct sdio_list hif_tx_data_sdio_list;
}hif_tx_list_t;


struct sdio_host_txdesc
{
    //tx len filled by host when use tx aggregation mode.
    uint16_t sdio_tx_len;
    // tx total len  used for host
    uint16_t sdio_tx_total_len;
    /// Pointer to packet payload
    uint8_t  packet_offset;
    // agg_num
    uint8_t agg_num;
    /// Size of the payload
    uint16_t packet_len;
};

struct sdio_host_rxdesc
{
    // payload offset
    uint16_t            pld_offset;
    /// Total length of the payload
    uint16_t            frmlen;
    /// total sdio rx len
    uint16_t            sdio_rx_len;
    uint16_t            num;
};
#define SDIO_HOAT_RXDESC_SIZE (sizeof(struct sdio_host_rxdesc))

struct rx_data_desc
{
    // HIF HDR struct
    hif_elmt_t hif_elmt;
    // HOST RX DESC
    struct sdio_host_rxdesc host_rx_desc;
    /// Buffer containing the payload
    uint32_t buffer[MAX_SDIO_TXRX_SIZE];
};

struct tx_data_desc
{
    hif_elmt_t hif_elmt;
    struct sdio_host_txdesc hostdesc;
    uint8_t tx_buf[MAX_SDIO_TXRX_SIZE];
};

/// Message structure.
struct sdio_msg
{
    struct sdio_list_hdr hdr;     ///< List header for chaining
    uint16_t        id;         ///< Message id.
    uint16_t        dest_id;    ///< Destination kernel identifier.
    uint16_t        src_id;     ///< Source kernel identifier.
    uint16_t        param_len;  ///< Parameter embedded struct length.
    uint32_t        param[];   ///< Parameter embedded struct. Must be word-aligned.
};

struct sdio_msg_desc
{
    hif_elmt_t hif_elmt;
    struct sdio_msg sdio_msg;
    uint8_t param[IPC_E2A_MSG_PARAM_SIZE + SDIO_HOST_TX_END_LEN + SDIO_BLKSZ];
};

/// Format of an event handler function
typedef void (*sdio_evt_ptr_t)(int);

/// Structure defining an event callback
struct sdio_evt_tag
{
    /// Pointer to the event function to call
    sdio_evt_ptr_t func;
    /// Parameter to pass to the event function
    int       param;
};

/// Kernel environment definition
struct sdio_env_tag
{
    /// Bit field indicating the events that are currently set
    volatile uint32_t evt_field;
};


static void host_rx_data_triger_evt(int dummy);
static void host_tx_data_evt(int dummy);
static void host_tx_msg_evt(int dummy);
static void host_rx_msg_fwd(const struct sdio_msg *sdio_msg);
static void hif_push_rx_elmt(uint8_t hif_type, uint8_t *addr, uint16_t len, uint8_t trigger);
static void hif_rx_comp_handler(int dummy);
static void hif_attach_tx_empty_elmt(uint8_t hif_type, uint8_t *addr, uint8_t trigger);
static void hif_tx_comp_handler(int dummy);
static void trigger_sdio_tx_timeout(void *env);
static void trigger_sdio_rx_timeout(void *env);
static void sdio_evt_set(uint32_t event);


static void hif_set_sdio_rx_elmt(uint8_t port_idx, uint32_t base_addr, uint16_t len,bool is_data);
static void hif_set_sdio_tx_elmt(uint8_t port_idx, uint32_t base_addr,bool is_data);
static void sdio_irq_enable(bool enable);

static void sdio_evt_clear(uint32_t const event);

#endif//CFG_SDIO_SUPPORT

#endif //RF_SDIO_H

