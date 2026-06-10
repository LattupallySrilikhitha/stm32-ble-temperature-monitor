#include"sensorManager.h"
#include "adc.h"
#include "GlobalDataBase.h"

static uint16_t CalculateAverage(uint16_t *buffer,
                                 uint16_t size);

static float Convert_ADC_To_Voltage(uint16_t adcValue);
static float Convert_Voltage_To_Temperature(float voltage);

void Sensor_StartSampling(void)
{
    HAL_ADC_Start_DMA(&hadc1,
                      (uint32_t*)adcBuffer,
                      ADC_BUFFER_SIZE);

}


void Sensor_StopSampling(void)
{
    HAL_ADC_Stop_DMA(&hadc1);
}


static uint16_t CalculateAverage(uint16_t *buffer,
                                 uint16_t size)
{
    uint32_t sum = 0;

    uint16_t index = 0;

    for(index = 0; index < size; index++)
    {
        sum += buffer[index];
    }

    return (uint16_t)(sum / size);
}

static float Convert_ADC_To_Voltage(uint16_t adcValue)
{
    float voltage = 0;

    voltage = ((float)adcValue * 3.3f) / 4095.0f;

    return voltage;
}

static float Convert_Voltage_To_Temperature(float voltage)
{
    float temperature = 0;

    temperature = ((voltage - 0.76f) / 0.0025f) + 25.0f;

    return temperature;
}
void ProcessTemperatureData(uint16_t *buffer,
                            uint16_t size)
{
    uint16_t averageADC = 0;

    float voltage = 0;

    float temperature = 0;

    averageADC = CalculateAverage(buffer, size);

    voltage = Convert_ADC_To_Voltage(averageADC);

    temperature = Convert_Voltage_To_Temperature(voltage);

    sensorDB.latestTemperature = temperature;

    sensorDB.temperatureHistory[sensorDB.historyIndex] = temperature;

    if(sensorDB.validSampleCount < TEMPERATURE_HISTORY_SIZE)
    {
        sensorDB.validSampleCount++;
    }

    sensorDB.historyIndex++;

    if(sensorDB.historyIndex >= TEMPERATURE_HISTORY_SIZE)
    {
        sensorDB.historyIndex = 0;
    }
}
