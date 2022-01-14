/*
 * File: Connection.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "Connection.h"


#include <ia20/iot/logger/LogLevel.h>

namespace IA20 {
namespace IOT {

/*************************************************************************/
Connection::Connection(
	unsigned int iListener,
    HandleType   aHandle,
	wchar_t* utfClientId):
	iListener(iListener),
	IdentifiedByHandle(aHandle),
	tabOutputActivies(CMaxActivitesPerConnection),
	tabInputActivies(CMaxActivitesPerConnection){	
	IA20_TRACER;

	if(utfClientId){
		wcsncpy(this->utfClientId, utfClientId, CMaxClientIdLen);
	}else{
		static uint64_t iID = 0; //TODO lib gen uid
		sprintf((char*)this->utfClientId, "auto-%036lX", iID++);
	}

}
/*************************************************************************/
Connection::~Connection() throw(){
	IA20_TRACER;
}
/*************************************************************************/
}
}
