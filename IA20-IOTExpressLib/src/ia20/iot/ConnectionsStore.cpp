/*
 * File: ConnectionsStore.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "ConnectionsStore.h"


namespace IA20 {
namespace IOT {

/*************************************************************************/
ConnectionsStore::ConnectionsStore():
iCounter(0){
	IA20_TRACER;
}
/*************************************************************************/
ConnectionsStore::~ConnectionsStore() throw(){
	IA20_TRACER;
}
/*************************************************************************/
Connection *ConnectionsStore::create(
			   unsigned int iListener, 
               wchar_t* utfClientId){
IA20_TRACER;

	ConnectionsPool::unique_ptr ptrResult(
		ConnectionsPool::New(iListener, Tools::IdentifiedByHandle::CUnset, utfClientId)
	);

	Tools::IdentifiedByHandle::HandleType aHandle = ConnectionsPool::PointerToIdx(ptrResult.get());

	aHandle = ++iCounter << 32 | (0xffffffff & aHandle);

	setHandle(*ptrResult, aHandle);
	
	return ptrResult.release();
}
/*************************************************************************/
Connection *ConnectionsStore::get(Connection::HandleType aHandle){
	IA20_TRACER;

	Connection *pConnection = ConnectionsPool::IdxToPonter(0xffffffff & aHandle);

	if(!pConnection || pConnection->getHandle() != aHandle){
		IA20_THROW(ItemNotFoundException("Connection"));
	}

	return pConnection;
}
/*************************************************************************/
}
}
