/*
 * File: MessageStore.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "MessageStore.h"


namespace IA20 {
namespace IOT {

/*************************************************************************/
MessageStore::MessageStore(){
	IA20_TRACER;
}

/*************************************************************************/
MessageStore::~MessageStore() throw(){
	IA20_TRACER;
}

/*************************************************************************/
const Message* MessageStore::createMessage(
		Memory::StreamBufferList::Reader& reader, uint32_t iDataLength){
	IA20_TRACER;

	size_t iSize = Message::ComputeRequiredMemory(iDataLength);
	
	Message *pMessage = reinterpret_cast<Message*>(
		new(std::aligned_alloc(alignof(Message), iSize))Message(iNextHandle, iDataLength));

	hmMessages[iNextHandle].reset(pMessage);

	IA20_LOG(true, "New Message: "<<(void*)(long)(iNextHandle)<<", len: "<<iDataLength);

  	IA20_LOG(true, "Packet data length: "<<iDataLength);
  	int rc = reader.copy(pMessage->getDataPointer(), iDataLength); 
 
  	IA20_LOG(true, "Data ["<<iDataLength<<":"<<rc<<"]"
        <<MiscTools::BinarytoHex((char*)pMessage->getData(),iDataLength));

	return hmMessages[iNextHandle++].get();
}
/*************************************************************************/
const Message* MessageStore::lookup(Message::HandleType aHandle)const{
	IA20_TRACER;

	IA20_LOG(true, "Lookup Message: "<<(void*)(long)(aHandle));

	MessagesMap::const_iterator it = hmMessages.find(aHandle);

	if(it == hmMessages.end())
		IA20_THROW(ItemNotFoundException("Message not found [lookup]: handle=")<<(void*)(long)aHandle);

	return it->second.get();
}
/*************************************************************************/
void MessageStore::dispose(const Message* pMessage){
	IA20_TRACER;

	Message::HandleType aHandle(pMessage->getHandle());

	IA20_LOG(true, "Dispose Message: "<<(void*)(long)(aHandle));

	MessagesMap::iterator it = hmMessages.find(aHandle);

	if(it == hmMessages.end())
		IA20_THROW(ItemNotFoundException("Message not found [erase]: handle=")<<(void*)(long)aHandle);

	hmMessages.erase(it);
}
/*************************************************************************/

}
}
