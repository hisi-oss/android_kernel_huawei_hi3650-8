/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and 
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may 
 * *    be used to endorse or promote products derived from this software 
 * *    without specific prior written permission.
 * 
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef __HIFIOMINTERFACE_H__
#define __HIFIOMINTERFACE_H__

/*****************************************************************************
  1 ??????????????
*****************************************************************************/
#include "vos.h"
#include "CodecInterface.h"

/* _H2ASN_Include OmCodecInterface.h */
/* _H2ASN_Include om_log.h */

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#pragma pack(4)

/*****************************************************************************
  2 ??????
*****************************************************************************/

/* OM-PC tool??????????????, ????CARM????OM_APP_HEADER_STRU */
#define OM_APP_HEADER VOS_UINT16 usFuncType;    \
                      VOS_UINT16 usLength;      \
                      VOS_UINT32 ulSn;          \
                      VOS_UINT32 ulTimeStamp;

#define OM_APP_HEADER_TAIL_LEN          (8)                 /* ulSn??ulTimerStamp????      */
#define OM_TRACE_TAIL_LEN               (4)                 /* usPrimId??usToolId???? */
#define OM_TRANS_FUNC_TYPE              (4)                 /* ????CARM????OM_FUNC_ID_ENUM */
#define OM_TRACE_FUNC_TYPE              (1)                 /* ????CARM????OM_FUNC_ID_ENUM */
#define OM_TRANS_PRIM_ID                (0x5001)            /* ????CARM????OM_TRANS_PRIMID */
#define OM_TRACE_PRIM_ID                (0x8003)            /* ????CARM????OM_APP_TRACE_IND*/


#define HIFI_SOCP_CHANNEL_START_ADDR    (0x33ffe000U)       /*HIFI SOCP??????????*/
#define HIFI_SOCP_CHANNEL_LEN           (0x2000)            /*HIFI SOCP????????*/
#define HIFI_SOCP_FIX_CHANNEL_ID        (29)

/*****************************************************************************
  3 ????????
*****************************************************************************/

/*****************************************************************************
 ?? ?? ??  : HIFI_OM_MSG_ID_ENUM
 ????????  : ????HiFi??????????????????
*****************************************************************************/
enum HIFI_OM_MSG_ID_ENUM
{
    ID_OM_CODEC_SET_LOG_REQ               = 0x940D,           /* ????????????????????????????*/ /* _H2ASN_MsgChoice OM_CODEC_SET_LOG_REQ_STRU */
    ID_OM_CODEC_EXT_DEF_REQ               = 0x9417,           /* ????????????????*/             /* _H2ASN_MsgChoice OM_CODEC_EXT_DEF_REQ_STRU */

    ID_OM_CODEC_MSG_HOOK_REQ              = 0x9420,                                             /* _H2ASN_MsgChoice OM_CODEC_MSG_HOOK_REQ_STRU */
    ID_OM_CODEC_CPU_VIEW_REQ              = 0x9421,                                             /* _H2ASN_MsgChoice OM_CODEC_CPU_VIEW_REQ_STRU */
    ID_OM_CODEC_OM_CONNECT_CMD            = 0x9422,
    ID_OM_CODEC_OM_DISCONNECT_CMD         = 0xc001,

    ID_AUDIODSP_DIAG_CONNECT_CNF          = 0x492A,
    ID_AUDIODSP_DIAG_DISCONNECT_CNF       = 0x492B,

    ID_TIMER_CODEC_CPUVIEW_RPT_IND        = 0x9401,

    ID_CODEC_OM_SET_LOG_CNF               = 0x490D,           /* ????OM_VOICE_SET_LOG_REQ*/     /* _H2ASN_MsgChoice CODEC_OM_SET_LOG_CNF_STRU */
    ID_CODEC_OM_EXT_DEF_CNF               = 0x4917,           /* ????OM_VOICE_EXT_DEF_REQ*/     /* _H2ASN_MsgChoice CODEC_OM_EXT_DEF_CNF_STRU */

    ID_CODEC_OM_MSG_HOOK_CNF              = 0x4920,                                             /* _H2ASN_MsgChoice CODEC_OM_MSG_HOOK_CNF_STRU */
    ID_CODEC_OM_CPU_VIEW_CNF              = 0x4921,                                             /* _H2ASN_MsgChoice CODEC_OM_CPU_VIEW_CNF_STRU */

    ID_CODEC_OM_CPUVIEW_BASIC_IND         = 0x4802,                                             /* _H2ASN_MsgChoice CODEC_OM_CPUVIEW_BASIC_IND_STRU */
    ID_CODEC_OM_CPUVIEW_DETAIL_IND        = 0x4803,                                             /* _H2ASN_MsgChoice CODEC_OM_CPUVIEW_DETAIL_IND_STRU */
    ID_CODEC_OM_BUTT
};
typedef VOS_UINT16 HIFI_OM_MSG_ID_ENUM_UINT16;

enum HIFI_OM_LOG_LEVEL_ENUM
{
    ID_CODEC_OM_LOG_INFO_IND              = 0x08000000,                                         /* _H2ASN_MsgChoice _H2ASN_Skip */
    ID_CODEC_OM_LOG_NORMAL_IND            = 0x10000000,                                         /* _H2ASN_MsgChoice _H2ASN_Skip */
    ID_CODEC_OM_LOG_WARNING_IND           = 0x20000000,                                         /* _H2ASN_MsgChoice _H2ASN_Skip */
    ID_CODEC_OM_LOG_ERR_IND               = 0x40000000,                                         /* _H2ASN_MsgChoice _H2ASN_Skip */
};
typedef VOS_UINT32 HIFI_OM_LOG_LEVEL_ENUM_UINT32;

/*****************************************************************************
 ??????    : OM_CODEC_EXT_CMD_ENUM
 ????????  : ??????????
*****************************************************************************/
enum OM_CODEC_EXT_CMD_ENUM
{
    OM_CODEC_EXT_CMD_DEFAULT  = 0,
    OM_CODEC_EXT_ENABLE_OM,
    OM_CODEC_EXT_VQM_CMD,
    OM_CODEC_EXT_HIFI_DDR_DFS,
    OM_CODEC_EXT_AUDIO_SET_SOURCE,
    OM_CODEC_EXT_UCOM_MEM_DYN_SET_OM_FLAG,
    OM_CODEC_EXT_VOICE_DIAGNOSE_START,
    OM_CODEC_EXT_VOICE_EVRC_CODEC,
    OM_CODEC_EXT_CDMA_LOOP_ENABLE,
    OM_CODEC_EXT_CDMA_LOOP_DISABLE,
    OM_CODEC_EXT_CHANGE_MIC_DMA,
    OM_CODEC_EXT_CHANGE_SPK_DMA,
    OM_CODEC_EXT_RESET_HIFI,
    OM_CODEC_EXT_CMD_BUT
};
typedef VOS_UINT16  OM_CODEC_EXT_CMD_ENUM_UINT16;

/*****************************************************************************
 ??????    : OM_LOG_RPT_LEVEL_ENUM_UINT16
 ????????  : HIFI????????????
*****************************************************************************/
enum OM_LOG_RPT_LEVEL_ENUM
{
    OM_LOG_LEVEL_INFO                   = 0 ,
    OM_LOG_LEVEL_WARNING,
    OM_LOG_LEVEL_ERROR,
    OM_LOG_LEVEL_BUTT
};
typedef VOS_UINT16 OM_LOG_RPT_LEVEL_ENUM_UINT16;

/*****************************************************************************
 ??????    : OM_SWITCH_ENUM
 ????????  : ??????????????
*****************************************************************************/
enum OM_SWITCH_ENUM
{
    OM_SWITCH_OFF                       = 0,                /* ???? */
    OM_SWITCH_ON,                                           /* ???? */
    OM_SWITCH_BUTT
};
typedef VOS_UINT16 OM_SWITCH_ENUM_UINT16;

/*****************************************************************************
 ??????    : OM_CPUVIEW_TARGET_ENUM
 ????????  : HIFI????????????
*****************************************************************************/
enum OM_CPUVIEW_TARGET_ENUM
{
    OM_CPUVIEW_TARGET_TASK              = 0,                /*??????????????*/
    OM_CPUVIEW_TARGET_INT               = 1,                /*??????????????*/
    OM_CPUVIEW_TARGET_REGION            = 2,                /*??????????????????*/
    OM_CPUVIEW_TARGET_BUTT
};
typedef VOS_UINT8 OM_CPUVIEW_TARGET_ENUM_UINT8;

/*****************************************************************************
 ??????    : OM_CPUVIEW_RECORD_TYPE_ENUM
 ????????  : HIFI????????????????
*****************************************************************************/
enum OM_CPUVIEW_ACTION_ENUM
{
    OM_CPUVIEW_ACTION_ENTER             = 0,                /*??????????????????*/
    OM_CPUVIEW_ACTION_EXIT              = 1,                /*??????????????????*/
    OM_CPUVIEW_ACTION_BUTT
};
typedef VOS_UINT8 OM_CPUVIEW_ACTION_ENUM_UINT8;

/*****************************************************************************
 ????????  : IPC_STATUS_HIFI2CARM_ENUM
 ????????  : ????????????????
*****************************************************************************/
enum IPC_STATUS_HIFI2CARM_ENUM
{
    IPC_STATUS_HIFI2CARM_MB             = 0x1,              /* HIFI??CARM?????????? */
    IPC_STATUS_HIFI2CARM_INIT           = 0x2,              /* HIFI??CARM?????????????? */
    IPC_STATUS_HIFI2CARM_PD             = 0x4,              /* HIFI??CARM?????????????? */
    IPC_STATUS_HIFI2CARM_BUTT           = 0x8
};
typedef VOS_UINT16 IPC_STATUS_HIFI2CARM_ENUM_UINT16;

/*****************************************************************************
 ????????  : COMM_VOICE_PCVOICE_DATA_DIRECTION_ENUM
 ????????  : PC_VOICE????????
*****************************************************************************/
enum COMM_VOICE_PCVOICE_DATA_DIRECTION_ENUM
{
    COMM_VOICE_PCVOICE_DATA_DIRECTION_TX = 0,               /* ????????: USB -> COMM -> VOICE -> PHY -> ???? */
    COMM_VOICE_PCVOICE_DATA_DIRECTION_RX,                   /* ????????: USB <- COMM <- VOICE <- PHY <- ???? */
    COMM_VOICE_PCVOICE_DATA_DIRECTION_BUTT,
};
typedef VOS_UINT32 COMM_VOICE_PCVOICE_DATA_DIRECTION_ENUM_UINT32;

/*****************************************************************************
 ????????  : HIFI_ERR_LOG_ALARM_ID_ENUM
 ????????  : HIFI??????????????????ID
*****************************************************************************/
enum HIFI_ERR_LOG_ALARM_ID_ENUM
{
    HIFI_ERR_LOG_ONE_WAY_NO_SOUND     = 0x1000,
    HIFI_ERR_LOG_VOICE_CONTROL_FAULT,
    HIFI_ERR_LOG_VOICE_SUSPEND_SLOW,
    HIFI_ERR_LOG_VOICE_BSD_ALARM,
    HIFI_ERR_LOG_VOICE_BSD_ALARM_SUB,
    HIFI_ERR_LOG_VOICE_BSD_WRR_NOISE,
    HIFI_ERR_LOG_VOICE_BSD_VQI_ALARM,
    HIFI_ERR_LOG_ALARM_ID_BUTT,
};
typedef VOS_UINT16 HIFI_ERR_LOG_ALARM_ID_ENUM_UINT16;

/*****************************************************************************
 ????????  : HFII_ERR_LOG_ALARM_LEVEL_ENUM
 ????????  : HIFI??????????????????
*****************************************************************************/
enum HFII_ERR_LOG_ALARM_LEVEL_ENUM
{
    HIFI_ERR_LOG_ALARM_CRITICAL     = 0x01,             /* ???? */
    HIFI_ERR_LOG_ALARM_MAJOR,                           /* ???? */
    HIFI_ERR_LOG_ALARM_MINOR,                           /* ???? */
    HIFI_ERR_LOG_ALARM_WARNING,                         /* ???? */
    HIFI_ERR_LOG_ALARM_LEVEL_BUTT,
};
typedef VOS_UINT16 HIFI_ERR_LOG_ALARM_LEVEL_ENUM_UINT16;

/*****************************************************************************
 ????????  : HIFI_ERR_LOG_ALARM_TYPE_ENUM
 ????????  : HIFI??????????????????????
*****************************************************************************/
enum HIFI_ERR_LOG_ALARM_TYPE_ENUM
{
    HIFI_ERR_LOG_ALARM_COMMUNICATION = 0x00,            /* ???? */
    HIFI_ERR_LOG_ALARM_TYPE_BUTT,
};
typedef VOS_UINT16 HIFI_ERR_LOG_ALARM_TYPE_ENUM_UINT16;

/*****************************************************************************
  4 ????????????
*****************************************************************************/

/*****************************************************************************
  5 ??????????
*****************************************************************************/

/*****************************************************************************
 ??????    : OM_TRACE_HEADER_STRU
 ????????  : TRACE??????????, ????CARM????OM_APP_TRACE_STRU
*****************************************************************************/
typedef struct
{
    OM_APP_HEADER
    VOS_UINT16                          usPrimId;           /* TRACE ID */
    VOS_UINT16                          usToolId;           /* reserve  */
}OM_TRACE_HEADER_STRU;

/*****************************************************************************
 ??????    : OM_TRANS_HEADER_STRU
 ????????  : OM??????????????, ????CARM????PS_OM_TRANS_IND_STRU
*****************************************************************************/
typedef struct
{
    OM_APP_HEADER
}OM_TRANS_HEADER_STRU;

/*****************************************************************************
 ??????    : OM_TRANS_PASS_HEADER_STRU
 ????????  : OM????????????CARM-OM??????????, ????CARM????PS_OM_TRANS_IND_STRU
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          usTransPrimId;      /* TRANS ID */
    VOS_UINT16                          usReserve;
    OM_APP_HEADER
}OM_TRANS_PASS_HEADER_STRU;

/*****************************************************************************
 ????????  : OM_SOCP_FRAME_HEAD_STRU
 ????????  : SOCP??????????
*****************************************************************************/
typedef struct
{
    VOS_UCHAR       ucCpuId;         /* CPU????????????0x5??????Hifi */      /*_H2ASN_Skip */
    VOS_UCHAR       aucRsv[3];       /* ????????????    */                /*_H2ASN_Skip */
    VOS_UINT32      ulRtc;           /* 32?????????????????? */
}OM_SOCP_FRAME_HEAD_STRU;

/*****************************************************************************
 ????????  : OM_SOCP_FRAME_SEG_STRU
 ????????  : SOCP????????????????
*****************************************************************************/
typedef struct
{
    VOS_UINT16      uhwSegLen;          /*??????????????????(????????)????????????????UsbTail??????byte*/
    VOS_UCHAR       ucSegNum;           /*??????????:Range[1,255] 1: ???????? other: ????????????????????????????????????1??????????*/ /*_H2ASN_Skip */
    VOS_UCHAR       ucSegSn;            /*??????????:Range[1,255]*/ /*_H2ASN_Skip */
}OM_SOCP_FRAME_SEG_STRU;

/*****************************************************************************
 ????????  : OM_SOCP_MSG_STRUs
 ????????  : SOCP????????????????
*****************************************************************************/
/* ????????????
typedef struct
{
    VOS_UINT32                          uwMagicNum;     'hisi'
    VOS_UINT16                          uhwMsgLen;      ????????
    VOS_UINT16                          uhwReserve;
    OM_SOCP_FRAME_HEAD_STRU             stFrmHead;      OM????
    VOS_UINT32                          uwUsbHead;      ??????0xaaaa5555
    OM_SOCP_FRAME_SEG_STRU              stFrmSegHead;   OM????????
    VOS_UINT32                          auwMsg[xxx];    ????????????,8????????
    VOS_UINT32                          uwUsbTail;      ??????0x5555aaaa
}OM_SOCP_MSG_STRU;
*/

/*****************************************************************************
  6 ????????
*****************************************************************************/

/*****************************************************************************
 ????????  : OM_CODEC_SET_LOG_REQ_STRU
 ????????  : MSG_OM_CODEC_SET_LOG_REQ????,????????????
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT16                          uhwMsgId;           /*_H2ASN_Skip*/
    VOS_UINT16                          uhwReserve1;
    OM_SWITCH_ENUM_UINT16               enLogEnable;        /* ???????? */
    OM_LOG_RPT_LEVEL_ENUM_UINT16        enLogLevel;         /* ???????? */
}OM_CODEC_SET_LOG_REQ_STRU;

/*****************************************************************************
 ????????  : CODEC_OM_SET_LOG_CNF_STRU
 ????????  : ID_CODEC_OM_SET_LOG_CNF????,????SDT????????????
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          uhwMsgId;           /*_H2ASN_Skip */
    VOS_UINT16                          uhwReserve;
    UCOM_RET_ENUM_UINT16                enReturn;           /* Request???????? */
    VOS_UINT16                          uhwReserve2;
}CODEC_OM_SET_LOG_CNF_STRU;

/*****************************************************************************
 ????????  : CODEC_OM_LOG_IND_STRU
 ????????  : ID_CODEC_OM_LOG_ERR_IND/ID_CODEC_OM_LOG_WARNING_IND/
             ID_CODEC_OM_LOG_INFO_IND????,????HIFI??????????
*****************************************************************************/
 typedef struct
{
    VOS_UINT16                          uhwMsgId;           /*_H2ASN_Skip */
    VOS_UINT16                          uhwReserve;
    VOS_UINT16                          uhwFileId;          /*_H2ASN_Replace OM_FILE_ID_ENUM_UINT16 enFileId; */
    VOS_UINT16                          uhwLineNo;
    VOS_UINT32                          uwLogId;            /*_H2ASN_Replace OM_LOG_ID_ENUM_UINT32  uwLogId; */
    VOS_INT32                           aswData[3];         /* 3??32BIT????*/
} CODEC_OM_LOG_IND_STRU;

/*****************************************************************************
 ????????  : OM_CODEC_CPU_VIEW_REQ_STRU
 ????????  : ??????????CODEC????????????????????????????????????????????????????????????
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT16                          uhwMsgId;           /*_H2ASN_Skip */
    VOS_UINT16                          uhwReserve1;
    OM_SWITCH_ENUM_UINT16               enEnable;
    VOS_UINT16                          uhwPeriod;          /* ????????????????,????1-5, ????20ms */
    OM_SWITCH_ENUM_UINT16               enDetailEnable;     /* ???????????????????????? */
    VOS_UINT16                          uhwReserve2;
} OM_CODEC_CPU_VIEW_REQ_STRU;

/*****************************************************************************
 ????????  : CODEC_OM_CPU_VIEW_CNF_STRU
 ????????  : ??????????ID_OM_CODEC_CPU_VIEW_REQ??????SDT
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          uhwMsgId;           /*_H2ASN_Skip */
    VOS_UINT16                          uhwReserve;
    UCOM_RET_ENUM_UINT16                enReturn;           /* REQ???????? */
    VOS_UINT16                          uhwReserve2;
} CODEC_OM_CPU_VIEW_CNF_STRU;

/*****************************************************************************
 ????????  : CODEC_OM_CPUVIEW_BASIC_IND_STRU
 ????????  : ID_CODEC_OM_CPUVIEW_BASIC_IND????,????HIFI??????????????
*****************************************************************************/
 typedef struct
{
    VOS_UINT16                          uhwMsgId;                               /*_H2ASN_Skip */
    VOS_UINT16                          usReserve;
    VOS_UINT32                          auwStackRate[HIFI_TASK_NUM];            /* ??????????????????????, ??????????????, Q8 */
    VOS_UINT32                          auwTaskTime[HIFI_TASK_NUM];             /* ?????????????????? */
    VOS_UINT32                          auwIntrTime[HIFI_INTR_NUM];             /* ?????????????????? */
} CODEC_OM_CPUVIEW_BASIC_IND_STRU;

/*****************************************************************************
 ????????  : OM_CPUVIEW_SLICE_RECORD_STRU
 ????????  : ??????????????????????????
 *****************************************************************************/
typedef struct
{
    VOS_UINT32                          uwTarget:2;                             /* ??????????, OM_CPUVIEW_TARGET_ENUM */
    VOS_UINT32                          uwTargetId:5;                           /* ????ID?????????????????? */
    VOS_UINT32                          uwAction:1;                             /* OM_CPUVIEW_ACTION_ENUM_UINT8 */
    VOS_UINT32                          uwTimeStamp:24;                         /* ????????????????????????????????24bit*/
}OM_CPUVIEW_SLICE_RECORD_STRU;

/*****************************************************************************
 ????????  : CODEC_OM_CPUVIEW_DETAIL_IND_STRU
 ????????  : ID_CODEC_OM_CPUVIEW_DETAIL_IND????,????????CPU????????????
 *****************************************************************************/
typedef struct
{
    VOS_UINT16                          uhwMsgId;                               /*_H2ASN_Skip */
    VOS_UINT16                          uhwReserve;
    VOS_UINT32                          uwRecNum;                               /* ???????? */
    OM_CPUVIEW_SLICE_RECORD_STRU        astRecords[1];                          /* ???????? */
} CODEC_OM_CPUVIEW_DETAIL_IND_STRU;

/*****************************************************************************
 ????????  : OM_CODEC_MSG_HOOK_REQ_STRU
 ????????  : ??????????CODEC????????HiFi 330??????????????????SDT
 *****************************************************************************/
 typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT16                          uhwMsgId;           /*_H2ASN_Skip*/
    VOS_UINT16                          uhwReserve;
    OM_SWITCH_ENUM_UINT16               enHookEnable;       /* 0-????, 1-????????????*/
    VOS_UINT16                          uhwReserve2;
}OM_CODEC_MSG_HOOK_REQ_STRU;

/*****************************************************************************
 ????????  : CODEC_OM_MSG_HOOK_CNF_STRU
 ????????  : ??????????ID_OM_CODEC_SET_HOOK_REQ??????SDT
 *****************************************************************************/
 typedef struct
{
    VOS_UINT16                          uhwMsgId;           /*_H2ASN_Skip */
    VOS_UINT16                          uhwReserve;
    UCOM_RET_ENUM_UINT16                enReturn;
    VOS_UINT16                          uhwReserve2;
}CODEC_OM_MSG_HOOK_CNF_STRU;

/*****************************************************************************
 ????????  : OM_CODEC_EXT_DEF_REQ_STRU
 ????????  : ????????????
 *****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT16                          uhwMsgId;           /*_H2ASN_Skip*/
    VOS_UINT16                          uhwReserve;
    OM_CODEC_EXT_CMD_ENUM_UINT16          enExtCmd;           /* ???????? */
    VOS_UINT16                          uhwExtLen;          /* ????????????, ????16bit */
    VOS_UINT16                          auhwExtData[2];     /* ????????????????????uhwExtLen */
}OM_CODEC_EXT_DEF_REQ_STRU;

/*****************************************************************************
 ????????  : CODEC_OM_EXT_DEF_CNF_STRU
 ????????  : ????OM_CODEC_EXT_DEF_REQ_STRU????
 *****************************************************************************/
typedef struct
{
    VOS_UINT16                          uhwMsgId;           /*_H2ASN_Skip*/
    VOS_UINT16                          uhwReserve;
    OM_CODEC_EXT_CMD_ENUM_UINT16          enExtCmd;           /* ?????????????? */
    VOS_UINT16                          uhwExtLen;          /* ????????????, ????16bit */
    VOS_UINT16                          auhwExtData[2];     /* ????????, ??????????uhwExtLen */
}CODEC_OM_EXT_DEF_CNF_STRU;

/*****************************************************************************
 ????????  : OM_CODEC_OM_CONNECT_CMD_STRU
 ????????  : ????Hifi????OM??????????
 *****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT16                          uhwMsgId;           /*_H2ASN_Skip*/
    VOS_UINT16                          uhwReserve;
}OM_CODEC_OM_CONNECT_CMD_STRU;

/*****************************************************************************
 ????????  : OM_CODEC_OM_DISCONNECT_CMD_STRU
 ????????  : ????Hifi????OM??????????
 *****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT16                          uhwMsgId;           /*_H2ASN_Skip*/
    VOS_UINT16                          uhwReserve;
}OM_CODEC_OM_DISCONNECT_CMD_STRU;

/*****************************************************************************
 ????????  : HIFI????DIAG????????????????
 ????????  : ??????????
 *****************************************************************************/
struct msg_om_audiodsp_diag_confirm
{
    VOS_MSG_HEADER
    VOS_UINT16                          msg_id;
    VOS_UINT16                          reserve;
};

/*****************************************************************************
  7 STRUCT????
*****************************************************************************/

/* RING BUFFER ?????? */
#define COMM_VOICE_RING_BUFFER_PROTECT1 (0x55AA55AA)        /* ??????1, ????????COMM_VOICE_RING_BUFFER_CONTROL_STRU */
#define COMM_VOICE_RING_BUFFER_PROTECT2 (0x5A5A5A5A)        /* ??????2, ????????COMM_VOICE_RING_BUFFER_CONTROL_STRU */

/*****************************************************************************
 ??????    : COMM_VOICE_RING_BUFFER_CONTROL_STRU
 ????????  : ring buffer??????????????
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          uwProtectWord1;                         /*?????? 0x55AA55AA*/
    VOS_UINT32                          uwProtectWord2;                         /*?????? 0x5A5A5A5A*/
    VOS_UINT32                          uwWriteAddr;                            /*????????????????????????,????????????Byte*/
    VOS_UINT32                          uwReadAddr;                             /*????????????????????????,????????????Byte*/
    VOS_UINT32                          uwBufSize;                              /*ring buffer??????,????byte */
    VOS_UINT32                          uwBufAddr;                              /*ring buffer??????(??????)*/
    VOS_UINT32                          uwProtectWord3;                         /*?????? 0x55AA55AA*/
    VOS_UINT32                          uwProtectWord4;                         /*?????? 0x5A5A5A5A*/
}COMM_VOICE_RING_BUFFER_CONTROL_STRU;

/*****************************************************************************
  8 UNION????
*****************************************************************************/


/*****************************************************************************
  9 OTHERS????
*****************************************************************************/
#if 0                                                       /* _H2ASN_Skip */

/*****************************************************************************
  H2ASN????????????????
*****************************************************************************/
/* ??????????????ASN????,????L????,??????V???? */
typedef struct
{
    HIFI_OM_MSG_ID_ENUM_UINT16          enMsgID;            /* _H2ASN_MsgChoice_Export HIFI_OM_MSG_ID_ENUM_UINT16 */

    VOS_UINT8                           aucMsgBlock[2];
    /***************************************************************************
        _H2ASN_MsgChoice_When_Comment          HIFI_OM_MSG_ID_ENUM_UINT16
    ****************************************************************************/
}VOICEOM_OM_TRANS_MSG_DATA;

/* ????????????*/
typedef struct
{
    VOICEOM_OM_TRANS_MSG_DATA             stMsgData;
}VoiceomOmTrans_MSG;

#endif                                                     /* _H2ASN_Skip */

/*****************************************************************************
  10 ????????
*****************************************************************************/

/*****************************************************************************
 ?? ?? ??  : COMM_VOICE_GetPcVoiceRingBuffCtrlAddr
 ????????  : ????RingBuff??????????????????
 ????????  : enDirection - ????????
 ????????  : ??
 ?? ?? ??  : Ring Buff????????????????(??????)
             ??Ring Buff??????????????????????????VOS_NULL

 ????????  : (1) Ring Buff????????????????????COMM_VOICE_RING_BUFFER_CONTROL_STRU
             (2) Ring Buff????????????????????????COMM??(??????2??, ??????????????????????????)
             (3) COMM????????????????????????????????????????????????RingBuff??????????

*****************************************************************************/
extern VOS_UINT32 COMM_VOICE_GetPcVoiceRingBuffCtrlAddr(COMM_VOICE_PCVOICE_DATA_DIRECTION_ENUM_UINT32 enDirection);

/*****************************************************************************
 ?? ?? ??  : COMM_VOICE_TransferPcVoiceRxData
 ????????  : ????????????????
 ????????  : ??
 ????????  : ??
 ?? ?? ??  : VOS_OK  - ????????
             VOS_ERR - ????????

 ????????  : (1) VOICE??????????????Ring Buff????????????????COMM????????????USB

*****************************************************************************/
extern VOS_UINT32 COMM_VOICE_TransferPcVoiceRxData(VOS_VOID);

/*****************************************************************************
 ?? ?? ??  : COMM_VOICE_SetPcVoiceTransferSize
 ????????  : ????PC Voice??????????????????, ??????????320 Byte
 ????????  : uwSize - ??????????????????
 ????????  : ??
 ?? ?? ??  : VOS_OK  - ????????
             VOS_ERR - ????????

 ????????  : (1) ????????????????????, ??????????, ??????????????320Byte; ??????????, ??????????????640Byte

*****************************************************************************/
extern VOS_UINT32 COMM_VOICE_SetPcVoiceTransferSize(VOS_UINT32 uwSize);

#if (VOS_OS_VER == VOS_WIN32)
#pragma pack()
#else
#pragma pack(0)
#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of HifiOmInterface.h */
