/*
 * File: VariableHeaderBuilder.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_MQTT_VariableHeaderBuilder_H_
#define _IA20_IOT_MQTT_VariableHeaderBuilder_H_

#include <ia20/commonlib/commonlib.h>
#include "Message.h"

namespace IA20 {
namespace IOT {
namespace MQTT {

/*************************************************************************/
/** The VariableHeaderBuilder class.
 *
 */
class VariableHeaderBuilder {
public:

	inline VariableHeaderBuilder():
		pCursor(data){};

	void buildFixedHeader(Message::Type iType, uint32_t iLength){

	}

	void finish();
	
protected:

	static const uint32_t CBuffLen = 4000;
	uint8_t  data[CBuffLen];

	uint8_t* pCursor;
	uint32_t iMaxSize;
};

/*************************************************************************/
}
}
}

#endif /* _IA20_IOT_MQTT_VariableHeaderBuilder_H_ */
