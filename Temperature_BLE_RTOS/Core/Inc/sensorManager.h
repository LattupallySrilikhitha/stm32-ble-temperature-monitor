#ifndef INC_SENSORMANAGER_H_
#define INC_SENSORMANAGER_H_

#include "main.h"

void Sensor_StartSampling(void);

void Sensor_StopSampling(void);

void ProcessTemperatureData(uint16_t *buffer,
                            uint16_t size);

#endif /* INC_SENSORMANAGER_H_ */
