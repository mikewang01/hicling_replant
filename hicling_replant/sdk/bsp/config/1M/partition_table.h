/*
 *****************************************************************************************
 *
 * Copyright (C) 2016. Dialog Semiconductor Ltd, unpublished work. This computer
 * program includes Confidential, Proprietary Information and is a Trade Secret of
 * Dialog Semiconductor Ltd. All use, disclosure, and/or reproduction is prohibited
 * unless authorized in writing. All Rights Reserved.
 *
 * <black.orca.support@diasemi.com> and contributors.
 *
 *****************************************************************************************
 */

PARTITION2( 0x000000 , 0x07F000 , NVMS_FIRMWARE_PART        , 0 )
PARTITION2( 0x07F000 , 0x001000 , NVMS_PARTITION_TABLE      , PARTITION_FLAG_READ_ONLY )
PARTITION2( 0x080000 , 0x010000 , NVMS_PARAM_PART           , 0 )
PARTITION2( 0x090000 , 0x030000 , NVMS_BIN_PART             , 0 )
PARTITION2( 0x0C0000 , 0 ,        NVMS_LOG_PART             , 0 )
PARTITION2( 0x0C0000 , 0x040000 , NVMS_GENERIC_PART         , 0 )//PARTITION_FLAG_VES )

