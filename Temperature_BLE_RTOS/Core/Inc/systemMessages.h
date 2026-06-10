/*
 * systemMessages.h
 *
 *  Created on: 21-May-2026
 *      Author: srilikhitha
 */

#ifndef INC_SYSTEMMESSAGES_H_
#define INC_SYSTEMMESSAGES_H_

typedef enum
{
    DEVICE_BLE = 0,

    DEVICE_WIFI

} DeviceId_t;


typedef enum
{
    PARAM_TEMPERATURE = 0

} ParameterId_t;


typedef enum
{
    ACCESS_CURRENT = 0,

    ACCESS_HISTORY,

    ACCESS_STREAM_START,

    ACCESS_STREAM_STOP

} AccessType_t;


typedef struct
{
    DeviceId_t deviceId;

    ParameterId_t parameterId;

    AccessType_t accessType;

} SystemMessage_t;

#endif /* INC_SYSTEMMESSAGES_H_ */
