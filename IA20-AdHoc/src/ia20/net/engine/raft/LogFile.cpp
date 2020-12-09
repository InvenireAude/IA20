/*
 * File: LogFile.cpp
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *
 */

#include "LogFile.h"

#include "LogEntry.h"

namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {

const char* LogFile::CTag = "iaLo";

/*************************************************************************/
LogFile::LogFile(size_t iSequneceId):
  iSequenceId(iSequenceId){
	IA20_TRACER;
}
/*************************************************************************/
String LogFile::CreateFileName(const String& strPath, ServerIdType iMyServerId, size_t iFileIdx){
  StringStream ss;
  ss<<strPath<<"/LOG-";
  ss<<TypeTools::IntToString(iMyServerId)<<"-";
  ss<<TypeTools::LongToString(iFileIdx)<<".log";
  return ss.str();
}
/*************************************************************************/
}
}
}
}
