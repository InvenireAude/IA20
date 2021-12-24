/*
 * File: HeaderReader.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_MQTT_HeaderReader_H_
#define _IA20_IOT_MQTT_HeaderReader_H_

#include <ia20/commonlib/commonlib.h>
#include <ia20/iot/tools/MQTT.h>

#include "Message.h"

namespace IA20 {
namespace IOT {
namespace MQTT {

/*************************************************************************/
/** The HeaderReader class.
 *
 */
class HeaderReader {
public:

	HeaderReader(uint8_t* pData):pData(pData){
		pAfterVL = Tools::MQTT::decodeVL(pData + 1, iLength);		
	};

	inline Message::Type getType()const{
		return (Message::Type)(0x0f & ((*pData) >> 4));
	}

	inline Message::Flag getAllFlags()const{
		return (Message::Flag)(0x0f & *pData);
	}

	inline bool testFlag(Message::Flag iFlag)const{
		return (Message::Flag)(0x0f & *pData) & iFlag;
	}

	inline uint8_t getQoS(){
		return ((0x0f & *pData) >> 1) & 0x3;		
	}

	inline uint32_t getLength()const{
		return iLength;
	}

	inline uint16_t getID()const{
		return (*pAfterVL) | (*(pAfterVL + 1) << 8);
	}

protected:
	uint8_t* pData;
	uint8_t* pAfterVL;
	uint32_t iLength;
};

/*************************************************************************/
}
}
}

#endif /* _IA20_IOT_MQTT_HeaderReader_H_ */
