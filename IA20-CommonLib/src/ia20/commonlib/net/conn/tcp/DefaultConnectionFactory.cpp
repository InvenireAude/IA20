/*
 * File: DefaultConnectionFactory.cpp
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *
 */

#include "DefaultConnectionFactory.h"

#include "FileHandle.h"

namespace IA20 {
namespace Net {
namespace Conn {
namespace TCP {

/*************************************************************************/
DefaultConnectionFactory::DefaultConnectionFactory(){
	IA20_TRACER;
}

/*************************************************************************/
DefaultConnectionFactory::~DefaultConnectionFactory() throw(){
	IA20_TRACER;
}
/*************************************************************************/
FileHandle* DefaultConnectionFactory::createFileHandle(int fd, const Peer& peer){
  IA20_TRACER;

  return new FileHandle(fd, peer);
}
/*************************************************************************/
}
}
}
}
