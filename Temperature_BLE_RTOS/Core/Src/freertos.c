/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "adc.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>
#include "GlobalDataBase.h"
#include "sensorManager.h"
#include "systemMessages.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for SystemTask */
osThreadId_t SystemTaskHandle;
const osThreadAttr_t SystemTask_attributes = {
  .name = "SystemTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for SensorTask */
osThreadId_t SensorTaskHandle;
const osThreadAttr_t SensorTask_attributes = {
  .name = "SensorTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for BLETask */
osThreadId_t BLETaskHandle;
const osThreadAttr_t BLETask_attributes = {
  .name = "BLETask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for sensorToBleQueue */
osMessageQueueId_t sensorToBleQueueHandle;
const osMessageQueueAttr_t sensorToBleQueue_attributes = {
  .name = "sensorToBleQueue"
};
/* Definitions for adcHalfSem */
osSemaphoreId_t adcHalfSemHandle;
const osSemaphoreAttr_t adcHalfSem_attributes = {
  .name = "adcHalfSem"
};
/* Definitions for adcFullSem */
osSemaphoreId_t adcFullSemHandle;
const osSemaphoreAttr_t adcFullSem_attributes = {
  .name = "adcFullSem"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
/* Definitions for bleToSensorQueue */
osMessageQueueId_t bleToSensorQueueHandle;

const osMessageQueueAttr_t bleToSensorQueue_attributes =
{
  .name = "bleToSensorQueue"
};
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void sensorTask(void *argument);
void bleTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of adcHalfSem */
  adcHalfSemHandle = osSemaphoreNew(1, 1, &adcHalfSem_attributes);

  /* creation of adcFullSem */
  adcFullSemHandle = osSemaphoreNew(1, 1, &adcFullSem_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of sensorToBleQueue */
  sensorToBleQueueHandle = osMessageQueueNew (10, 16, &sensorToBleQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */

  /* creation of sensorToBleQueue */
   bleToSensorQueueHandle = osMessageQueueNew (10, sizeof(SystemMessage_t), &bleToSensorQueue_attributes);
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of SystemTask */
  SystemTaskHandle = osThreadNew(StartDefaultTask, NULL, &SystemTask_attributes);

  /* creation of SensorTask */
  SensorTaskHandle = osThreadNew(sensorTask, NULL, &SensorTask_attributes);

  /* creation of BLETask */
  BLETaskHandle = osThreadNew(bleTask, NULL, &BLETask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the SystemTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_sensorTask */
/**
* @brief Function implementing the SensorTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_sensorTask */
void sensorTask(void *argument)
{
  /* USER CODE BEGIN sensorTask */
	SystemMessage_t receivedMessage;

	SystemMessage_t responseMessage;
  /* Infinite loop */
  for(;;)
  {
	  osMessageQueueGet(bleToSensorQueueHandle,
	                    &receivedMessage,
	                    NULL,
	                    osWaitForever);

	  switch(receivedMessage.parameterId)
	  {
	      case PARAM_TEMPERATURE:

	          switch(receivedMessage.accessType)
	          {
	              case ACCESS_CURRENT:
	            	  Sensor_StartSampling();

	            	  osSemaphoreAcquire(adcHalfSemHandle,
	            	                     osWaitForever);
	            	  memcpy(&rawTempDataBuffer[0],
	            	         &adcBuffer[0],
	            	         (ADC_BUFFER_SIZE / 2) * sizeof(uint16_t));

	            	  osSemaphoreAcquire(adcFullSemHandle,
	            	                     osWaitForever);

	            	  memcpy(&rawTempDataBuffer[ADC_BUFFER_SIZE / 2],
	            	         &adcBuffer[ADC_BUFFER_SIZE / 2],
	            	         (ADC_BUFFER_SIZE / 2) * sizeof(uint16_t));

	            	  Sensor_StopSampling();

	            	  ProcessTemperatureData(rawTempDataBuffer,
	            	                         ADC_BUFFER_SIZE);

	            	  responseMessage.deviceId = DEVICE_BLE;

	            	  responseMessage.parameterId = PARAM_TEMPERATURE;

	            	  responseMessage.accessType = ACCESS_CURRENT;

	            	  osMessageQueuePut(sensorToBleQueueHandle,
	            	                    &responseMessage,
	            	                    0,
	            	                    0);
	            	  break;



	              case ACCESS_HISTORY:

	                  break;

	              case ACCESS_STREAM_START:

	                  break;

	              case ACCESS_STREAM_STOP:

	                  break;

	              default:

	                  break;
	          }

	          break;

	      default:

	          break;
	  }

  }
  /* USER CODE END sensorTask */
}

/* USER CODE BEGIN Header_bleTask */
/**
* @brief Function implementing the BLETask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_bleTask */
void bleTask(void *argument)
{
  /* USER CODE BEGIN bleTask */
	SystemMessage_t sensorRequest;
	SystemMessage_t sensorResponse;
	int sendRequest = 1;

  /* Infinite loop */
  for(;;)
  {
	  sensorRequest.deviceId = DEVICE_BLE;

	  sensorRequest.parameterId = PARAM_TEMPERATURE;

	  sensorRequest.accessType = ACCESS_CURRENT;

if(sendRequest)
	  osMessageQueuePut(bleToSensorQueueHandle,
	                    &sensorRequest,
	                    0,
	                    0);

	  osMessageQueueGet(sensorToBleQueueHandle,
	                    &sensorResponse,
	                    NULL,
	                    osWaitForever);
	  sendRequest = 0;//temporary, for debugging purpose.
  }
  /* USER CODE END bleTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

