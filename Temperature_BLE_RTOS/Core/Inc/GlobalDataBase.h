/*
 * GlobalDataBase.h
 *
 *  Created on: 15-May-2026
 *      Author: srilikhitha
 */

#ifndef INC_GLOBALDATABASE_H_
#define INC_GLOBALDATABASE_H_

#include "main.h"

#define ADC_BUFFER_SIZE 200
#define TEMPERATURE_HISTORY_SIZE   24





typedef struct
{
    float latestTemperature;

    float temperatureHistory[TEMPERATURE_HISTORY_SIZE];

    uint16_t historyIndex;

    uint16_t validSampleCount;

} SensorDataBase_t;

extern SensorDataBase_t sensorDB;

extern uint16_t adcBuffer[ADC_BUFFER_SIZE];

extern uint16_t rawTempDataBuffer[ADC_BUFFER_SIZE];


#endif /* INC_GLOBALDATABASE_H_ */
