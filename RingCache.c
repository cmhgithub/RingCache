/*
* This file is part of the RingCache Library.
*
* Copyright (c) 2019-2019, cmhwp, <cmhwp@live.com>
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* 'Software'), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
* Created on: 2019-12-19
*/

#include "RingCache.h"

/* MSG RingBuff */
#define NEXT_MSG(X) ((X)++, (X) = ((X) >= RING_MSG_NUM ? 0 : (X)))
/* 将数据放入RingMsg */
void RingMsg_free(RING_MSG * p, uint8_t msg_index)
{
	if (p->len[msg_index] != 0) {//此位置确实空闲，可以释放
		p->free += p->len[msg_index];//归还空间
		p->start[msg_index] = 0;
		p->len[msg_index] = 0;
	}
}

void RingMsg_init(RING_MSG * p)
{
	p->free = 256;
	p->index = 0;
	p->BufWritePoint = 0;

	for (uint8_t msg_index = 0; msg_index < RING_MSG_NUM; msg_index++) {
		p->start[msg_index] = 0;
		p->len[msg_index] = 0;
	}
}

uint32_t RingMsg_InLen(RING_MSG * p, const uint8_t * _data, uint32_t inLen)
{
	uint8_t free_index;

	/* 参数检查 */
	if (inLen > 255) {
		return 0;
	}

	/* 释放足够的空间 */
	free_index = p->index;
	while (p->free < inLen) {
		RingMsg_free(p, free_index);
		NEXT_MSG(free_index);
	}

	/* 入缓存 */
	p->start[p->index] = p->BufWritePoint;
	for (uint8_t i = 0; i < inLen; i++) {
		p->Buf[(uint8_t)(p->start[p->index] + i)] = _data[i];
	}
	p->len[p->index] = inLen;
	p->BufWritePoint += inLen;
	p->free -= inLen;
	NEXT_MSG(p->index);

	return 0;
}

/* 将数据移出RingMsg */
uint32_t RingMsg_OutLen(RING_MSG * p, uint8_t * _data)
{
	uint8_t read_index = p->index;
	uint8_t read_len = 0;
	for (uint8_t msg_index = 0; msg_index < RING_MSG_NUM; msg_index++) {
		if (p->len[read_index] != 0) {
			for (uint8_t i = 0; i < p->len[read_index]; i++) {
				_data[i] = p->Buf[(uint8_t)(p->start[read_index] + i)];
			}
			read_len = p->len[read_index];
			RingMsg_free(p, read_index);
			return read_len;
		}
		else {
			NEXT_MSG(read_index);
		}
	}

	return 0;
}

