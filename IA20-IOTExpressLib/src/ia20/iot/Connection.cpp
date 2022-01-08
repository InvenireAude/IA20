/*
 * File: Connection.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "Connection.h"


namespace IA20 {
namespace IOT {

/*************************************************************************/
Connection::Connection(
	unsigned int iListener,
    HandleType   aHandle,
	wchar_t* utfClientId):
	iListener(iListener),
	IdentifiedByHandle(aHandle){	
	IA20_TRACER;

	wcsncpy(this->utfClientId, utfClientId, CMaxClientIdLen);
}
// /*************************************************************************/
// Connection::~Connection() throw(){
// 	IA20_TRACER;
// }
/*************************************************************************/
}
}
