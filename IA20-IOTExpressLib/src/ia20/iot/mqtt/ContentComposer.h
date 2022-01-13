/*
 * File: ContentComposer.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_MQTT_ContentComposer_H_
#define _IA20_IOT_MQTT_ContentComposer_H_

#include <ia20/commonlib/commonlib.h>
#include <ia20/iot/memory/StreamBufferList.h>

namespace IA20 {
namespace IOT {
namespace MQTT {

/*************************************************************************/
/** The ContentComposer class.
 *
 */
class ContentComposer {
public:

	ContentComposer():iTop(0){};

	inline void add(const uint8_t *pData, uint16_t iDataLenght){
		
		if(iTop == CMaxEntries)
			IA20_THROW(BadUsageException("iTop == CMaxEntries"));

		tabEntries[iTop].pData       = pData;
		tabEntries[iTop].iDataLenght = iDataLenght;
		
		iTop++;
	}

	inline void build(Memory::StreamBufferList::Writer& writer)const{
		
		for(int i = 0; i < iTop; i++){
			writer.write(tabEntries[i].pData, tabEntries[i].iDataLenght);
		}
			
	}

	inline uint32_t computeLength()const{
		uint32_t iLength = 0;
		
		for(int i = 0; i < iTop; i++){
			iLength += tabEntries[i].iDataLenght;
		}
			
		return iLength;
	}

protected:

	struct Entry{
		const uint8_t  *pData;
		uint16_t        iDataLenght;
	};
	
	static const int CMaxEntries = 15;

	Entry tabEntries[CMaxEntries];

	uint8_t iTop;
};
/*************************************************************************/
}
}
}

#endif /* _IA20_IOT_MQTT_ContentComposer_H_ */
