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
Logger::Logger(const Configuration& configuration, ServerIdType iMyServerId):
  configuration(configuration),
  iMyServerId(iMyServerId),
  iFileIdx(0){
	IA20_TRACER;
}

/*************************************************************************/
Logger::~Logger() throw(){
	IA20_TRACER;
}
/*************************************************************************/
static inline String _createFileName(const String& strPath, ServerIdType iMyServerId, size_t iFileIdx){
  StringStream ss;
  ss<<strPath<<"/LOG";
  ss<<TypeTools::IntToString(iMyServerId)<<"/";
  ss<<TypeTools::LongToString(iFileIdx)<<".log";
  return ss.str();
}
/*************************************************************************/
void Logger::allocateFileIfNeeded(LogEntrySizeType iNewEntryDataSize){

	IA20_TRACER;

  if(ptrActiveFile && LogEntry::ComputeSpace(iNewEntryDataSize) )
    return;

  std::unique_ptr<LogFile> ptrNewLogFile(new LogFile(_createFileName(configuration.strPath, iMyServerId, iFileIdx + 1), configuration.iLogFileSize));

  //TODO keep old files until can be rolled and realeased.

  ptrActiveFile = (std::move(ptrNewLogFile));

  iFileIdx++;
}
/*************************************************************************/
const LogEntry* Logger::appendEntry(TermType  iTerm, IndexType iIndex, LogEntrySizeType  iEntryDataSize, const void* pSrcData){
	IA20_TRACER;
  allocateFileIfNeeded(iEntryDataSize);

  return ptrActiveFile->appendEntry(iTerm, iIndex, iEntryDataSize, pSrcData);
}
/*************************************************************************/
void Logger::commit(const LogEntry* pLogEntry){
	IA20_TRACER;
  allocateFileIfNeeded(0);

  return ptrActiveFile->commit(pLogEntry);
}
/*************************************************************************/
}
}
}
}
