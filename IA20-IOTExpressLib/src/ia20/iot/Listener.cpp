/*
 * File: Listener.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "Listener.h"


#include <ia20/iot/logger/LogLevel.h>
#include <ia20/iot/tools/sys/TaskPort.h>

namespace IA20 {
namespace IOT {

/*************************************************************************/
Listener::Listener(){
	IA20_TRACER;
}
/*************************************************************************/
Listener::~Listener() throw(){
	IA20_TRACER;
}
/*************************************************************************/
void Listener::setupPort(){
	ptrRingHandler.reset(new URing::RingHandler);
	Tools::SYS::PipeFD::PeerType peerClient(aPipeFD.getClientPeer());
    ptrPort = Tools::SYS::TaskPort<Listener::Task*>::Create(
		1000, ptrRingHandler.get(), peerClient.first, peerClient.second);
}
/*************************************************************************/
}
}
