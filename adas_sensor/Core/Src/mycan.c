#include "mycan.h"


// 필터 설정
void filter_INIT(uint32_t f_maskid, uint32_t f_id)
{
	// 수신필터 설정은 normal 모드에 들어가기전 초기화 모드에서 설정해줘야 하기 때문에
	// HAL_CAN_Start() 함수 호출 전에 완료해야 함
	canFilter.FilterMode = CAN_FILTERMODE_IDMASK;
	canFilter.FilterScale = CAN_FILTERSCALE_16BIT;
	canFilter.FilterMaskIdHigh = f_maskid;
	canFilter.FilterIdHigh = f_id;
	canFilter.FilterMaskIdLow = f_maskid;
	canFilter.FilterIdLow = f_id;
	canFilter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	canFilter.FilterBank = 0;
	canFilter.FilterActivation = ENABLE;
}

// 송신 헤더 설정
// Txheader_SET(0x102, 8);
void Txheader_SET(uint32_t StandId, uint32_t len)
{
	TxHeader.StdId = StandId;       // Standard Identifier, 0 ~ 0x7FF
	TxHeader.RTR = CAN_RTR_DATA;    // 전송하는 메세지의 프레임 타입, DATA or REMOTE
	TxHeader.IDE = CAN_ID_STD;      // 전송하는 메세지의 식별자 타입, STD or EXT
	TxHeader.DLC = len;             // 송신 프레임 길이, 0 ~ 8 byte
}

void Can_SEND(uint32_t StandId, uint32_t len, uint8_t *TxData)
{
	Txheader_SET(StandId, len);

	TxMailbox = HAL_CAN_GetTxMailboxesFreeLevel(&hcan);
	HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox);
}

