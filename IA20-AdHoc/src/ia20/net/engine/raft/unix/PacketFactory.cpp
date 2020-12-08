/*
 * File: PacketFactory.cpp
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *
 */

#include "PacketFactory.h"


namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {
namespace Unix {
// TODO Use slabs allocator .
static const int _padding = 64;
/*************************************************************************/
PacketFactory::PacketFactory(){
	IA20_TRACER;
}
/*************************************************************************/
PacketFactory::~PacketFactory() throw(){
	IA20_TRACER;
}
/*************************************************************************/
void PacketFactory::allocatePacket(Packet& packet){
  IA20_TRACER;
  uint8_t *pResult = (uint8_t *)malloc(CMaxPacketSize + _padding);

  if(!pResult)
    IA20_THROW(SystemException("Cannot allocate memory for network packet."));

  IA20_LOG(LogLevel::INSTANCE.isDetailedInfo(), "Raft :: Malloc("<<(void*)pResult<<")");

  packet.set(pResult + _padding, pResult);
}
/*************************************************************************/
void PacketFactory::freeCtx(void *pCtx){
  IA20_TRACER;

  IA20_LOG(LogLevel::INSTANCE.isDetailedInfo(), "Raft :: Free("<<pCtx<<")");

  if(pCtx)
    free(pCtx);

}
/*************************************************************************/
}
}
}
}
}