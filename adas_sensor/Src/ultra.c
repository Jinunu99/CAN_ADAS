#include "ultra.h"
static uint8_t ultra_idx = 5;

ULTRA ultra[ULTRA_NUM] =
{
	{&htim3, TIM_CHANNEL_1, 0, 0, 0, 0, 0, 0},
	{&htim3, TIM_CHANNEL_2, 0, 0, 0, 0, 0, 0},
	{&htim3, TIM_CHANNEL_3, 0, 0, 0, 0, 0, 0},
	{&htim3, TIM_CHANNEL_4, 0, 0, 0, 0, 0, 0}
};


void ultra_init()
{
	HAL_TIM_Base_Start(&htim2); // timer start 해야함 -> delay us 사용을 위함

	// input capture가 생기면 인터럽트를 걸어야지
	HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2);
	HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_3);
	HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_4);
}

// 초음파 동작 방식 - 트리거를 발생시키면 에코의 길이를 출력해서 초음파의 길이를 측정함

void HCSR04_Trigger()
{
	// 안넣어도 동작하지만 확실하게 처음에 low로 설정
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 0);
	delay_us(1);

	// trigger에 10us 동안 High를 주고 Low로 떨어뜨림
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 1);
	delay_us(10);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 0);

	// timer 3번의 각 채널을 enable 시켜줌
	__HAL_TIM_ENABLE_IT(ultra[0].htim, TIM_IT_CC1);
	__HAL_TIM_ENABLE_IT(ultra[1].htim, TIM_IT_CC2);
	__HAL_TIM_ENABLE_IT(ultra[2].htim, TIM_IT_CC3);
	__HAL_TIM_ENABLE_IT(ultra[3].htim, TIM_IT_CC4);
}


// echo pulse output을 수행하는 함수
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	switch(htim->Channel)
	{
	case HAL_TIM_ACTIVE_CHANNEL_1:
		ultra_idx = 0;
		break;
	case HAL_TIM_ACTIVE_CHANNEL_2:
		ultra_idx = 1;
		break;
	case HAL_TIM_ACTIVE_CHANNEL_3:
		ultra_idx = 2;
		break;
	case HAL_TIM_ACTIVE_CHANNEL_4:
		ultra_idx = 3;
		break;
	}

	if (ultra[ultra_idx].captureFlag == 0)
	{
		// 첫 번째 캡처 (Rising Edge)
		ultra[ultra_idx].IC_Value1 = HAL_TIM_ReadCapturedValue(htim, ultra[ultra_idx].tim_channel);
		ultra[ultra_idx].captureFlag = 1;

		// Falling Edge로 변경
		__HAL_TIM_SET_CAPTUREPOLARITY(htim, ultra[ultra_idx].tim_channel, TIM_INPUTCHANNELPOLARITY_FALLING);
	}
	else
	{
		// 두 번째 캡처 (Falling Edge)
		ultra[ultra_idx].IC_Value2 = HAL_TIM_ReadCapturedValue(htim, ultra[ultra_idx].tim_channel);

		// 시간 계산
		if (ultra[ultra_idx].IC_Value2 > ultra[ultra_idx].IC_Value1)
			ultra[ultra_idx].echoTime = ultra[ultra_idx].IC_Value2 - ultra[ultra_idx].IC_Value1;
		else
			ultra[ultra_idx].echoTime = (0xFFFF - ultra[ultra_idx].IC_Value1) + ultra[ultra_idx].IC_Value2;

		// 거리 계산
		ultra[ultra_idx].distance = ultra[ultra_idx].echoTime / 58;

		// 캡처 상태 초기화 및 Rising Edge로 복귀
		ultra[ultra_idx].captureFlag = 0;
		__HAL_TIM_SET_CAPTUREPOLARITY(htim, ultra[ultra_idx].tim_channel, TIM_INPUTCHANNELPOLARITY_RISING);

		// 해당 채널 인터럽트 비활성화
		switch (ultra[ultra_idx].tim_channel)
		{
		case TIM_CHANNEL_1:
			__HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);
			break;
		case TIM_CHANNEL_2:
			__HAL_TIM_DISABLE_IT(htim, TIM_IT_CC2);
			break;
		case TIM_CHANNEL_3:
			__HAL_TIM_DISABLE_IT(htim, TIM_IT_CC3);
			break;
		case TIM_CHANNEL_4:
			__HAL_TIM_DISABLE_IT(htim, TIM_IT_CC4);
			break;
		}
	}
}

