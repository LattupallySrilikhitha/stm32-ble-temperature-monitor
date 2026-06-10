/*
 * rtosNotify.c
 *
 *  Created on: 21-May-2026
 *      Author: srilikhitha
 */

#include "rtosNotify.h"

#include "cmsis_os.h"
#include "freertos.h"

extern osSemaphoreId_t adcHalfSemHandle;

extern osSemaphoreId_t adcFullSemHandle;

void NotifyAdcHalfComplete(void)
{
    osSemaphoreRelease(adcHalfSemHandle);
}

void NotifyAdcFullComplete(void)
{
    osSemaphoreRelease(adcFullSemHandle);
}
