/*
 * File: ActionsStore.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "ActionsStore.h"


namespace IA20 {
namespace IOT {

const size_t ActionsStore::Action::CAlignedSize(
	sizeof(std::aligned_storage_t< sizeof(ActionsStore::Action), alignof(ActionsStore::Action) >));

/*************************************************************************/
ActionsStore::ActionsStore():
iNextHandle(0){
	IA20_TRACER;
}
/*************************************************************************/
ActionsStore::~ActionsStore() throw(){
	IA20_TRACER;
}
/*************************************************************************/
const ActionsStore::Action* ActionsStore::createAction(
		Memory::StreamBufferList::Reader& reader, uint32_t iDataLength){
	IA20_TRACER;

	size_t iSize = Action::ComputeRequiredMemory(iDataLength);
	
	Action *pAction = reinterpret_cast<Action*>(new(std::aligned_alloc(iSize,alignof(Action)))Action(iNextHandle));
	
	hmActions[iNextHandle].reset(pAction);

	IA20_LOG(true, "New Action: "<<(void*)(long)(iNextHandle)<<", len: "<<iDataLength);

  	IA20_LOG(true, "Packet data length: "<<iDataLength);
  	int rc = reader.copy(pAction->getDataPointer(), iDataLength); 
 
  	IA20_LOG(true, "Data ["<<iDataLength<<":"<<rc<<"]"
        <<MiscTools::BinarytoHex((char*)pAction->getData(),iDataLength));

	return hmActions[iNextHandle++].get();
}
/*************************************************************************/
const ActionsStore::Action* ActionsStore::lookup(Action::HandleType aHandle)const{
	IA20_TRACER;

	IA20_LOG(true, "Lookup Action: "<<(void*)(long)(aHandle));

	ActionsMap::const_iterator it = hmActions.find(aHandle);

	if(it == hmActions.end())
		IA20_THROW(ItemNotFoundException("Action not found [lookup]: handle=")<<(void*)(long)aHandle);

	return it->second.get();
}
/*************************************************************************/
void ActionsStore::dispose(const ActionsStore::Action* pAction){
	IA20_TRACER;

	Action::HandleType aHandle(pAction->getHandle());

	IA20_LOG(true, "Dispose Action: "<<(void*)(long)(aHandle));

	ActionsMap::iterator it = hmActions.find(aHandle);

	if(it == hmActions.end())
		IA20_THROW(ItemNotFoundException("Action not found [erase]: handle=")<<(void*)(long)aHandle);

	hmActions.erase(it);
}
/*************************************************************************/

}
}
