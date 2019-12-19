
ifndef _RING_CACHE_H_
#define RING_CACHE_H_

/* MSG RingBuff*/
#define RING_MSG_NUM 20

#if RING_MSG_NUM > 255
#error RING_MSG_NUM must less 256!!!
#endif

typedef struct {
	uint8_t Buf[256];
	uint8_t BufWritePoint;

	uint8_t start[RING_MSG_NUM];
	uint8_t len[RING_MSG_NUM];

	uint16_t free;

	uint8_t index;
}RING_MSG;


void RingMsg_init(RING_MSG * p);

/* 将数据放入RingMsg */
uint32_t RingMsg_InLen(RING_MSG * p, const uint8_t * _data, uint32_t inLen);

/* 将数据移出RingMsg */
uint32_t RingMsg_OutLen(RING_MSG * p, uint8_t * _data);

#endif
