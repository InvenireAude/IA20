/*
 * File: FixedHeaderBuilder.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_MQTT_FixedHeaderBuilder_H_
#define _IA20_IOT_MQTT_FixedHeaderBuilder_H_

#include <ia20/commonlib/commonlib.h>
#include "Message.h"
#include <ia20/iot/tools/MQTT.h>
#include <ia20/iot/memory/StreamBufferList.h>

namespace IA20 {
namespace IOT {
namespace MQTT {

/*************************************************************************/
/** The FixedHeaderBuilder class.
 *
 */
class FixedHeaderBuilder {
public:

	static const uint32_t CMaxLen = 8;

	inline FixedHeaderBuilder():
	iType(Message::MT_NONE),iFlags(Message::MF_NONE),iID(0){};

	inline void setType(Message::Type iType){
		this->iType = iType;
	}

	inline void setFlags(Message::Flag iFlags, uint8_t iQoS){
		uint8_t f = (uint8_t)iFlags;
		f &= (uint8_t)0x6;
		f |= ((uint8_t)0x3 & iQoS) << 1;
		this->iFlags = (Message::Flag)f;
	}
	
	inline void setLength(uint32_t iLength){
		this->iLength = iLength;
	}

	inline uint8_t* build(uint8_t* pCursor){		
		
		*pCursor++  = (0xf0 & (iType << 4)) | (0x0f & iFlags);
		pCursor     = Tools::MQTT::encodeVL(pCursor, iLength);
		return pCursor;
	}

	inline void build(Memory::StreamBufferList::Writer& writer){
		uint8_t buf[16];
		uint8_t* pCursor = build(buf);
		writer.write(buf, pCursor - buf);	
	}

protected:

	inline uint8_t getQoS(){
		return (iFlags >> 1) & 0x3;		
	}

	Message::Type  iType;
	Message::Flag  iFlags;

	uint16_t iID;
	uint32_t iLength;
};

/*************************************************************************/
}
}
}

#endif /* _IA20_IOT_MQTT_FixedHeaderBuilder_H_ */
