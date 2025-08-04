#ifndef INC_ULTRA_H_
#define INC_ULTRA_H_

#include "main.h"
#include "delay.h"

typedef struct
{
	TIM_HandleTypeDef 	*htim;	// 에코핀 타이머
	uint32_t 	tim_channel;	// 에코핀 채널
	uint16_t 	IC_Value1;  	// 에코핀이 High 엣지일 때
	uint16_t 	IC_Value2;    	// 에코핀이 Low 엣지일 때
	uint16_t 	echoTime;     	// 에코핀의 High 엣지와 Low 엣지 사이의 시간 (카운트 갯수)
	uint8_t 	captureFlag;    // high로 되었을때 capture되면 flag를 세움
	uint16_t 	distance;       // 거리
	uint8_t 	ultra_flag;		// 콜백함수 불러지면 1로 초기화
}ULTRA;

#define ULTRA_NUM 	4

void ultra_init();
void HCSR04_Trigger();

#endif /* INC_ULTRA_H_ */
