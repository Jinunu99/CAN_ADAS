/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
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
#include <stdio.h>
#include "usart.h"
#include "mycan.h"
#include "ir.h"
#include "ultra.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

int _write(int file, char* p, int len)
{
	HAL_UART_Transmit(&huart2, p, len, 10);
	return len;
}

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

extern ULTRA ultra[ULTRA_NUM];

//uint8_t TxData_ultra[8];
//uint8_t TxData_ir[8];


typedef enum
{
	SENSOR_ULTRA,
	SENSOR_IR
} SensorType;

typedef struct
{
	SensorType type;
	uint8_t data[8];
} SensorPacket;



/* USER CODE END Variables */
/* Definitions for ultraTask */
osThreadId_t ultraTaskHandle;
const osThreadAttr_t ultraTask_attributes = {
  .name = "ultraTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for irTask */
osThreadId_t irTaskHandle;
const osThreadAttr_t irTask_attributes = {
  .name = "irTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for transTask */
osThreadId_t transTaskHandle;
const osThreadAttr_t transTask_attributes = {
  .name = "transTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for sensorQueue */
osMessageQueueId_t sensorQueueHandle;
const osMessageQueueAttr_t sensorQueue_attributes = {
  .name = "sensorQueue"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void ultraTASK(void *argument);
void irTASK(void *argument);
void transTASK(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

	printf("STM32 start_sensor part!!\n");

	// 초음파 초기화
	ultra_init();


	/* CAN 설정 부분 */
	// 필터 설정
	filter_INIT(0x0000, 0x0000);

	// can 채널에 필터 적용
	HAL_CAN_ConfigFilter(&hcan, &canFilter);

	// 송수신을 하기 위해 Normal 모드 진입
	HAL_CAN_Start(&hcan);



  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of sensorQueue */
	sensorQueueHandle = osMessageQueueNew (16, sizeof(SensorPacket), &sensorQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of ultraTask */
  ultraTaskHandle = osThreadNew(ultraTASK, NULL, &ultraTask_attributes);

  /* creation of irTask */
  irTaskHandle = osThreadNew(irTASK, NULL, &irTask_attributes);

  /* creation of transTask */
  transTaskHandle = osThreadNew(transTASK, NULL, &transTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_ultraTASK */
/**
 * @brief  Function implementing the ultraTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_ultraTASK */
void ultraTASK(void *argument)
{
  /* USER CODE BEGIN ultraTASK */
	SensorPacket packet;

	/* Infinite loop */
	for(;;)
	{
		//memset(packet.data, 0, sizeof(packet.data));
		memset(&packet, 0, sizeof(packet));
		HCSR04_Trigger(); // 초음파 측정 시작

		printf("fst: %dcm, snd: %dcm, thd: %dcm, fth: %dcm\n",
			ultra[0].distance, ultra[1].distance, ultra[2].distance, ultra[3].distance);

		//		TxData_ultra[0] = ultra[0].distance;
		//		TxData_ultra[1] = ultra[1].distance;
		//		TxData_ultra[2] = ultra[2].distance;
		//		TxData_ultra[3] = ultra[3].distance;


		packet.type = SENSOR_ULTRA;
		packet.data[0] = ultra[0].distance;
		packet.data[1] = ultra[1].distance;
		packet.data[2] = ultra[2].distance;
		packet.data[3] = ultra[3].distance;


		// osMessageQueuePut - 데이터를 큐에 넣는 함수

		osStatus_t status;
		if((status = osMessageQueuePut(sensorQueueHandle, &packet, 0, 0)) != osOK)
		{
			if(status == osErrorResource)
				printf("Queue Full: sensorQueue is full!\n");
			else
				printf("Queue Error: osMessageQueuePut failed with code %d\n", status);
		}

		osDelay(200);
	}
  /* USER CODE END ultraTASK */
}

/* USER CODE BEGIN Header_irTASK */
/**
 * @brief Function implementing the irTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_irTASK */
void irTASK(void *argument)
{
  /* USER CODE BEGIN irTASK */
	SensorPacket packet;

	/* Infinite loop */
	for(;;)
	{
		/* PC8 - 왼, PC9 - 오 (검은색 on, 흰색 off)*/

//		// 왼쪽 IR
//		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) == 1)
//		{
//			TxData_ir[0] = 1;
//			printf("Left_ON\n");
//		}
//		else
//		{
//			TxData_ir[0] = 0;
//			printf("Left_OFF\n");
//		}
//
//		// 오른쪽 IR
//		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9) == 1)
//		{
//			TxData_ir[1] = 1;
//			printf("Right_ON\n");
//		}
//		else
//		{
//			TxData_ir[1] = 0;
//			printf("Right_OFF\n");
//		}

		//memset(packet.data, 0, sizeof(packet.data));
		memset(&packet, 0, sizeof(packet));

		packet.type = SENSOR_IR;
		// 검은색 - 1, 이외 - 0
		packet.data[0] = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) ? 1 : 0;
		packet.data[1] = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9) ? 1 : 0;

		osStatus_t status;
		if((status = osMessageQueuePut(sensorQueueHandle, &packet, 0, 0)) != osOK)
		{
			if(status == osErrorResource)
				printf("Queue Full: sensorQueue is full!\n");
			else
				printf("Queue Error: osMessageQueuePut failed with code %d\n", status);
		}


		osDelay(200);
	}
  /* USER CODE END irTASK */
}

/* USER CODE BEGIN Header_transTASK */
/**
 * @brief Function implementing the transTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_transTASK */
void transTASK(void *argument)
{
  /* USER CODE BEGIN transTASK */
	SensorPacket packet;

	/* Infinite loop */
	for(;;)
	{
		if(osMessageQueueGet(sensorQueueHandle, &packet, NULL, osWaitForever) == osOK)
		{
			if(packet.type == SENSOR_ULTRA)
			{
				Can_SEND(0x102, 4, packet.data);  // 초음파
				printf("ULTRA Packet Send!\n");
			}
			else if(packet.type == SENSOR_IR)
			{
				Can_SEND(0x106, 2, packet.data);  // IR
				printf("IR Packet Send!\n");
			}
		}

	}
  /* USER CODE END transTASK */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

