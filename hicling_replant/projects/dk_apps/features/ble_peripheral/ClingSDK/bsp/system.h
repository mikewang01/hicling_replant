//
//  File: system.h
//  
//  Description: System layer header
//
//  Created on Feb 26, 2014
//
#ifndef __SYSTEM_HEADER__
#define __SYSTEM_HEADER__

#include "standards.h"

#define SYSTEM_INFORMATION_BACKUP_IN_MS    60000 // Backup in every minute

void SYSTEM_CLING_init(void);


BOOLEAN SYSTEM_get_mutex(I8U value);
void SYSTEM_release_mutex(I8U value);
BOOLEAN SYSTEM_backup_critical(void);
void SYSTEM_get_dev_id(I8U *twentyCharDevID);
void SYSTEM_get_ble_code(I8U *code);
I8U SYSTEM_get_ble_device_name(I8U *device_name);
void SYSTEM_checksum(void *pData, I32U Size, I8U *checksum);
void SYSTEM_restart_from_reset_vector(void);
I16U SYSTEM_CRC(I8U *data, I32U len, I16U seed);
void SYSTEM_factory_reset(void);
void SYSTEM_format_disk(BOOLEAN b_erase_data);
void SYSTEM_reboot(void);

#endif
