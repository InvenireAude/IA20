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

/*************************************************************************/
LogFile::LogFile(const String& strName, size_t iSize):
  strName(strName),
  iSize(iSize){
	IA20_TRACER;

  pMemory = malloc(iSize); // TODO use MMAP on a pre allocated file.

  if(!pMemory)
    IA20_THROW(SystemException("Cannot allocate LogFile memory."));

  pNext = pMemory;
  iSpaceLeft = iSize;
}
/*************************************************************************/
LogFile::~LogFile() throw(){
	IA20_TRACER;
  free(pMemory);
}
/*************************************************************************/
const LogEntry* LogFile::appendEntry(TermType  iTerm, IndexType iIndex, LogEntrySizeType  iEntryDataSize, const void* pSrcData){
  IA20_TRACER;

  //TODO configurable asset as this is a double check ...

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: appendEntry: "<<LogEntry::ComputeSpace(iEntryDataSize)<<" ? "<<iSpaceLeft);

  if(LogEntry::ComputeSpace(iEntryDataSize) > iSpaceLeft)
    IA20_THROW(InternalException("LogEntry::ComputeSpace(iEntryDataSize) > iSpaceLeft"));

  LogEntry* pEntry = new (pNext) LogEntry(iTerm, iIndex, iEntryDataSize, pSrcData);

  pNext = pEntry->next();
  //TODO somewhat optimal method for iSpaceLeft :)
  iSpaceLeft = iSize - (reinterpret_cast<uint8_t*>(pNext) - reinterpret_cast<uint8_t*>(pMemory));
  return pEntry;
}
/*************************************************************************/
void LogFile::commit(const LogEntry* pLogEntry){
	IA20_TRACER;

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: commit: "<<*pLogEntry);
}
/*************************************************************************/
}
}
}
}
