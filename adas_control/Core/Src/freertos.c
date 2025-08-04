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
#include "led.h"
#include "motor.h"
#include "mycan.h"
#include "handle.h"
#include "car.h"


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

extern CAR car;

/* USER CODE END Variables */
/* Definitions for motorTask */
osThreadId_t motorTaskHandle;
const osThreadAttr_t motorTask_attributes = {
  .name = "motorTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for buzTask */
osThreadId_t buzTaskHandle;
const osThreadAttr_t buzTask_attributes = {
  .name = "buzTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for recvTask */
osThreadId_t recvTaskHandle;
const osThreadAttr_t recvTask_attributes = {
  .name = "recvTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void motorTASK(void *argument);
void buzTASK(void *argument);
void recvTASK(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

	printf("STM32 control Start!!\n");

	// 핸들 초기화
	handle_INIT();

	// 모터 초기화
	motor_INIT();

	// LED 초기화
	//led_INIT();


	/* CAN 설정 부분 */
	// 필터 설정
	filter_INIT(0x7F3, 0x106);

	// can 채널에 필터 적용
	HAL_CAN_ConfigFilter(&hcan, &canFilter);

	// 수신되면 인터럽트가 발생할 수 있도록
	//HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);

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

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of motorTask */
  motorTaskHandle = osThreadNew(motorTASK, NULL, &motorTask_attributes);

  /* creation of buzTask */
  buzTaskHandle = osThreadNew(buzTASK, NULL, &buzTask_attributes);

  /* creation of recvTask */
  recvTaskHandle = osThreadNew(recvTASK, NULL, &recvTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_motorTASK */
/**
 * @brief  Function implementing the motorTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_motorTASK */
void motorTASK(void *argument)
{
  /* USER CODE BEGIN motorTASK */
	/* Infinite loop */
	for(;;)
	{
		// 라인트레이서

		speed_SET(); 		// 모터 속도 설정
		direction_SET();	// 차량 방향 설정

		uint8_t speed = car.motor_speed;
		uint8_t dir = car.direction;

//		motor_front();
//		motor_speed(350);
//		osDelay(50);
//		motor_left();
//		motor_speed(350);
//		osDelay(50);
//		motor_right();
//		motor_speed(350);
//		osDelay(50);
//		motor_rear();
//		motor_speed(350);
//		osDelay(50);

		printf("dir : %d\n", dir);
		printf("speed : %d\n", speed);
		if(dir == STRAIGHT)
		{
			motor_front(400);
			//motor_speed(speed);
		}
		if(dir == LEFT)
		{
			motor_left(400);
			//motor_speed(speed * 2 / 3);
		}
		if(dir == RIGHT)
		{
			motor_right(400);
			//motor_speed(speed * 2 / 3);
		}
		if(dir == BACK)
		{
			motor_rear(400);
			//motor_speed(speed * 2 / 3);
		}

		osDelay(10);

		//osDelay(1000);
	}
  /* USER CODE END motorTASK */
}

/* USER CODE BEGIN Header_buzTASK */
/**
 * @brief Function implementing the buzTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_buzTASK */
void buzTASK(void *argument)
{
  /* USER CODE BEGIN buzTASK */
	/* Infinite loop */
	for(;;)
	{
		osDelay(1);
	}
  /* USER CODE END buzTASK */
}

/* USER CODE BEGIN Header_recvTASK */
/**
 * @brief Function implementing the recvTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_recvTASK */
void recvTASK(void *argument)
{
  /* USER CODE BEGIN recvTASK */
	/* Infinite loop */
	for(;;)
	{
		//		led_SET(255, 255, 0);   // 노란색
		//		osDelay(1000);
		//		led_SET(255, 120, 0);   // 주황색
		//		osDelay(1000);
		//		led_SET(255, 0, 0);     // 빨간색
		//		osDelay(1000);

		// 수신 FIFO0에 데이터가 있는지 확인 (폴링방식 수신)
		if (HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0) > 0)
		{
			// 수신 메시지 읽기
			if (HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK)
			{
				printf("CAN RX: ID=0x%03lX DATA=%d %d %d %d %d %d %d %d\n",
						RxHeader.StdId, RxData[0], RxData[1], RxData[2], RxData[3], RxData[4], RxData[5], RxData[6], RxData[7]);

				if(RxHeader.StdId == 0x102) // 초음파
				{
					car.ultra[0] = RxData[0]; // 전방
					car.ultra[1] = RxData[1]; // 왼쪽 측후방
					car.ultra[2] = RxData[2]; // 후방
					car.ultra[3] = RxData[3]; // 오른쪽 측후방

				}
				else if(RxHeader.StdId == 0x106) // ir
				{
					car.ir[0] = RxData[0]; // 왼쪽
					car.ir[1] = RxData[1]; // 오른쪽
				}
			}
		}
	}
  /* USER CODE END recvTASK */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

