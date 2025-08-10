#ifndef INC_MYCAN_H_
#define INC_MYCAN_H_

#include "main.h"
#include "can.h"


void filter_INIT(uint32_t f_maskid, uint32_t f_id);
void Txheader_SET(uint32_t StandId, uint32_t len);
void Can_SEND(uint32_t StandId, uint32_t len, uint8_t *TxData);


#endif /* INC_MYCAN_H_ */
