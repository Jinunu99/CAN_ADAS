#include "mycan.h"


// 필터 설정
// filter_INIT(0x7F3, 0x106);
void filter_INIT(uint32_t f_maskid, uint32_t f_id)
{
	// 수신필터 설정은 normal 모드에 들어가기전 초기화 모드에서 설정해줘야 하기 때문에
	// HAL_CAN_Start() 함수 호출 전에 완료해야 함
	canFilter.FilterMode = CAN_FILTERMODE_IDMASK;
	canFilter.FilterScale = CAN_FILTERSCALE_16BIT;
	canFilter.FilterMaskIdHigh = f_maskid << 5;
	canFilter.FilterIdHigh = f_id << 5;
	canFilter.FilterMaskIdLow = f_maskid << 5;
	canFilter.FilterIdLow = f_id << 5;
	canFilter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	canFilter.FilterBank = 0;
	canFilter.FilterActivation = ENABLE;
}

// 인터럽트 방식 수신
//void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
//{
//	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData);
//
//	// 모든 값이 0이면 무시
//	if (RxHeader.StdId == 0x000)
//		return;
//
//	printf("CAN RX: ID=0x%03lX DATA=%d %d %d %d %d %d %d %d\n",
//			RxHeader.StdId, RxData[0], RxData[1], RxData[2], RxData[3], RxData[4], RxData[5], RxData[6], RxData[7]);
//}
