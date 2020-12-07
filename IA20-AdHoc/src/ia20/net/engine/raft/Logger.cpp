/*
 * File: Logger.cpp
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *
 */

#include "Logger.h"

#include "LogEntry.h"
#include "LogFile.h"

namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {

/*************************************************************************/
Logger::Logger(const Configuration& configuration):
  configuration(configuration),
  iFileIdx(0){
	IA20_TRACER;
}

/*************************************************************************/
Logger::~Logger() throw(){
	IA20_TRACER;
}
/*************************************************************************/
static inline String _createFileName(const String& strPath,  size_t iFileIdx){
  return strPath + "/LOG" + TypeTools::LongToString(iFileIdx) + ".log";
}
/*************************************************************************/
void Logger::allocateFileIfNeeded(LogEntrySizeType iNewEntryDataSize){

  if(ptrActiveFile && LogEntry::ComputeSpace(iNewEntryDataSize) )
    return;

  std::unique_ptr<LogFile> ptrNewLogFile(new LogFile(_createFileName(configuration.strPath, iFileIdx + 1), configuration.iLogFileSize));

  //TODO keep old files until can be rolled and realeased.

  ptrActiveFile = (std::move(ptrNewLogFile));

  iFileIdx++;
}
/*************************************************************************/
void Logger::appendEntry(TermType  iTerm, IndexType iIndex, LogEntrySizeType  iEntryDataSize, const void* pSrcData){

}
/*************************************************************************/
}
}
}
}
