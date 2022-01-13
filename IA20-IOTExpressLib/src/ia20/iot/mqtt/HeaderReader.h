/*
 * File: HeaderReader.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_MQTT_HeaderReader_H_
#define _IA20_IOT_MQTT_HeaderReader_H_

#include <ia20/commonlib/commonlib.h>
#include <ia20/iot/memory/StreamBufferList.h>
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

	HeaderReader(IOT::Memory::StreamBufferList::Reader& reader):
		pData(reader.getData()){
		pAfterVL = Tools::MQTT::decodeVL(pData + 1, iLength);		
		
		Message::Type iType = getType();

		uint8_t iSkip = pAfterVL - pData + 
			(iType >= Message::MT_PUBACK && iType <= Message::MT_UNSUBACK ||
	 		 iType == Message::MT_PUBLISH && getQoS() >= 1) ? 2 : 0;
	
		reader.advance(iSkip);
	};

	HeaderReader(uint8_t* pData):
		pData(pData){
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

	uint32_t getRemainingLength(const IOT::Memory::StreamBufferList::Reader& reader){
		return iLength + (pAfterVL - pData) - reader.getConsumedBytes();
	};
	
	uint32_t getTotalLength(){
		return iLength + (pAfterVL - pData);
	};

	static uint16_t ReadTwoBytes(IOT::Memory::StreamBufferList::Reader& reader){
		uint8_t msb(reader.readByte());
		uint8_t lsb(reader.readByte());
		return ((uint16_t)msb << 8) + lsb;
	};

	static inline uint32_t DecodeVL(IOT::Memory::StreamBufferList::Reader& reader){

     uint32_t m = 1;
     uint32_t iValue = 0;

     uint8_t iByte;

     do {

      iByte   = reader.readByte();
      iValue += (iByte & 0x7f) * m;
      
      if (m > 128*128*128)
        IA20_THROW(InternalException("Add exception for decodeVL"));
     
      m *= 128;

     }while ((iByte & 0x80) != 0);

	  return iValue;
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
