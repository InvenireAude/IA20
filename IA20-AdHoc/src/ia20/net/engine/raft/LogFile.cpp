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
LogFile::LogFile(const String& strFileName):
  pFirstEntry(NULL),
  pLastEntry(NULL),
  strFileName(strFileName){
	IA20_TRACER;
}
/*************************************************************************/
String LogFile::CreateFileName(const String& strPath, ServerIdType iMyServerId){
  StringStream ss;
  ss<<strPath<<"/LOG-";
  ss<<TypeTools::IntToString(iMyServerId)<<".log";
  return ss.str();
}
/*************************************************************************/
}
}
}
}
