/*
 * File: Connection.cpp
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *
 */

#include "Connection.h"
#include <ia20/commonlib/net/conn/Address.h>

namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {
namespace Unix {

/*************************************************************************/
Connection::Connection(const String& strInterface, const String& strGroup, uint16_t iGroupPort):
  ptrSender(new Net::Conn::MCast::Sender(iGroupPort)),
  ptrReceiver(new Net::Conn::MCast::Receiver(iGroupPort)){
	IA20_TRACER;

  ptrSender->setup(strInterface, strGroup);
  ptrReceiver->subscribe(strInterface, strGroup);
  ptrReceiver->bind();

}
/*************************************************************************/
Connection::~Connection() throw(){
	IA20_TRACER;
}
/*************************************************************************/
void Connection::send(const Packet& packet){
	IA20_TRACER;
  ptrSender->send(packet.getDataStart(), packet.getLength());
}
/*************************************************************************/
bool Connection::receive(Packet& packet){
	IA20_TRACER;

  void     *pData = packet.getDataStart();
  uint32_t iDataSize;
  Net::Conn::Address address;

  bool bResult = ptrReceiver->receive(pData, iDataSize, address);
  packet.setLength(iDataSize);

  return bResult;
}
/*************************************************************************/
}
}
}
}
}
