/*
 * File: PropertiesComposer.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_MQTT_PropertiesComposer_H_
#define _IA20_IOT_MQTT_PropertiesComposer_H_

#include <ia20/commonlib/commonlib.h>
#include <ia20/iot/memory/StreamBufferList.h>
#include <ia20/iot/tools/StringRef.h>

namespace IA20 {
namespace IOT {
namespace MQTT {

 inline uint16_t hostToNet(uint16_t v){
 	return (0xff & (v >> 8)) | ((0xff & v) << 8);
 };

/*************************************************************************/
/** The PropertiesComposer class.
 *
 */
class PropertiesComposer {
public:

	PropertiesComposer():iTop(0){};

	inline void add(uint8_t iProperty, const uint8_t *pData, uint16_t iDataLenght, bool bIsString = false){
		
		if(iTop == CMaxEntries)
			IA20_THROW(BadUsageException("iTop == CMaxEntries"));

		tabEntries[iTop].iProperty   = iProperty;
		tabEntries[iTop].pData       = pData;
		tabEntries[iTop].iDataLenght = iDataLenght;
		tabEntries[iTop].bIsString   = bIsString;
		iTop++;
	}

	inline void add(uint8_t iProperty, const Tools::StringRef& strValue){
		add(iProperty, strValue.getData(), strValue.getLength(), true );
	}

	inline void build(Memory::StreamBufferList::Writer& writer)const{
		
		//TODO variable lenght > 128
		writer.writeByte(computeLength() - 1);

		for(int i = 0; i < iTop; i++){

			const Entry& e(tabEntries[i]);
			writer.writeByte(e.iProperty);

			if(e.bIsString){
				writer.writeTwoBytes(e.iDataLenght);
			}

			writer.write(e.pData, e.iDataLenght);
		}
			
	}

	inline uint32_t computeLength()const{
		uint32_t iLength = 0;
		
		for(int i = 0; i < iTop; i++){
			iLength += 1 + tabEntries[i].iDataLenght;
			if(tabEntries[i].bIsString){
				iLength += 2;
			}
		}
			
		return iLength + 1;
	}

protected:

	struct Entry{
		uint8_t         iProperty;
		const uint8_t  *pData;
		uint16_t        iDataLenght;
		bool            bIsString;
	};
	
	static const int CMaxEntries = 8;

	Entry tabEntries[CMaxEntries];

	uint8_t iTop;
};
/*************************************************************************/
}
}
}

#endif /* _IA20_IOT_MQTT_PropertiesComposer_H_ */
