/*
 * File: Logger.cpp
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *
 */

#include "Logger.h"
#include <ia20/commonlib/net/conn/Address.h>

#include "../LogEntry.h"
#include <unistd.h>

namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {
namespace Mocker {

/*************************************************************************/
Logger::Logger(ServerIdType iServerId, size_t iMemorySize):
    LogFileWriter("/tmp/.ia20.raft.tmp."+TypeTools::IntToString(iServerId)+"."+TypeTools::IntToString(::getpid()), iMemorySize){
  IA20_TRACER;
}
/*************************************************************************/
Logger::~Logger() throw(){
	IA20_TRACER;
  //simpleDump(std::cerr);
  //unlink(strFileName.c_str());
}
/*************************************************************************/
void Logger::simpleDump(std::ostream& os){
  IA20_TRACER;

  const LogEntry* pCursor = reinterpret_cast<const LogEntry*>(pFirstEntry);

  while(pCursor < pNextEntry){
    os<<pCursor->getEntryId();
    if(pCursor->getEntryDataSize()){
      os<<"{"<<String((const char*)pCursor->getData(),pCursor->getEntryDataSize())<<"}";
    }
    if(pCursor == pLastCommit)
    os<<"*";

    pCursor = pCursor->next();
  }
}
/*************************************************************************/
}
}
}
}
}
