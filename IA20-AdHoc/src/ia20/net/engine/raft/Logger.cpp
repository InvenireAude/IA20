/*
 * File: Logger.cpp
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *
 */

#include "Logger.h"

#include "LogEntry.h"

#include "LogFileWriter.h"
#include "LogFileAllocator.h"

namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {

/*************************************************************************/
Logger::Logger(const Configuration& configuration, ServerIdType iMyServerId):
  configuration(configuration),
  iMyServerId(iMyServerId){
	IA20_TRACER;
}

/*************************************************************************/
Logger::~Logger() throw(){
	IA20_TRACER;
}
/*************************************************************************/
void Logger::AllocateFile(const String& strFileName, size_t iSequenceId, size_t iSize){
  IA20_TRACER;

  std::unique_ptr<LogFileAllocator> ptrLogFileAllocator(
    new LogFileAllocator(strFileName, iSequenceId, iSize)
  );

}
/*************************************************************************/
void Logger::nextFileIfNeeded(LogEntrySizeType iNewEntryDataSize){

	IA20_TRACER;

  if(!empty() &&
     (*begin())->getSpaceLeft() >= LogEntry::ComputeSpace(iNewEntryDataSize) )
    return;

  size_t iSequenceId = empty() ? 0 : getActiveLogFile()->getSequenceId();
  iSequenceId++;

  String strFileName(LogFile::CreateFileName(configuration.strPath, iMyServerId, iSequenceId));

  AllocateFile(strFileName, iSequenceId, configuration.iLogFileSize);

  append(new LogFileWriter(strFileName, iSequenceId));
}
/*************************************************************************/
const LogEntry* Logger::appendEntry(TermType  iTerm, IndexType iIndex, LogEntrySizeType  iEntryDataSize, const void* pSrcData){
	IA20_TRACER;

  nextFileIfNeeded(iEntryDataSize);
  return getActiveLogFile()->appendEntry(iTerm, iIndex, iEntryDataSize, pSrcData);
}
/*************************************************************************/
const LogFileWriter* Logger::getActiveLogFile()const{
  return const_cast<Logger*>(this)->getActiveLogFile();
}
/*************************************************************************/
LogFileWriter* Logger::getActiveLogFile(){
    IA20_TRACER;
    if(empty())
      IA20_THROW(InternalException("No active LogFile allocated."));

    return getNext()->getValue();
}
/*************************************************************************/
void Logger::commit(const LogEntry* pLogEntry){
	IA20_TRACER;
  const_cast<LogEntry*>(pLogEntry)->commit(); // Yes, LogEntry user can't but we can ;)
  getActiveLogFile()->commit(pLogEntry); // TODO this may not be from this file :)))
}
/*************************************************************************/
const LogEntry* Logger::getLastLogEntryOrNull()const{
  IA20_TRACER;

  if(empty())
    return NULL;

  return getActiveLogFile()->getLastLogEntryOrNull();
}
/*************************************************************************/
}
}
}
}
