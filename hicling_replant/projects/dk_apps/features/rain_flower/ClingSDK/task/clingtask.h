/**
 ****************************************************************************************
 *
 * @file clingtask.h
 *
 * @brief hicling task service
 *
 * Copyright (C) 2016. hicling elec Ltd, unpublished work. This computer
 * program includes Confidential, Proprietary Information and is a Trade Secret of
 * Dialog Semiconductor Ltd.  All use, disclosure, and/or reproduction is prohibited
 * unless authorized in writing. All Rights Reserved.
 *
 * <black.orca.support@diasemi.com> and contributors.
 *
 ****************************************************************************************
 */

#ifndef CLING_TASK_H
#define CLING_TASK_H


#include <stdint.h>

/**
 * Register cling TASK instance
 *
 *
 * \param [in] pvParameters   parameter passed to this task
 *
 * \return null
 *
 */
void cling_task(void *pvParameters);

#endif /* CLING_TASK_H */
