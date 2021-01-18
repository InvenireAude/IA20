/*
 * File: TCPAcceptor.cpp
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *
 */

#include "TCPAcceptor.h"


namespace IA20 {
namespace URing {
namespace EV {

/*************************************************************************/
TCPAcceptor::TCPAcceptor(Net::Conn::TCP::Server* pServer):
pServer(pServer){
	IA20_TRACER;
}

/*************************************************************************/
TCPAcceptor::~TCPAcceptor() throw(){
	IA20_TRACER;
}
/*************************************************************************/
}
}
}
