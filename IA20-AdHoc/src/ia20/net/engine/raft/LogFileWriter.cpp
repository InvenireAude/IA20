/*
 * File: LogFileWriter.cpp
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *
 */

#include "LogFileWriter.h"

#include "LogEntry.h"

#include "LogFileAllocator.h"

namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {

/*************************************************************************/
LogFileWriter::LogFileWriter(const String& strFileName, size_t iMemorySize):
  LogFile(strFileName),
  pLastCommit(NULL){

	IA20_TRACER;

  //TODO this is stupid ;)
  if(iMemorySize){
    std::unique_ptr<Raft::LogFileAllocator>ptrLogFileAllocator(new Raft::LogFileAllocator(strFileName, iMemorySize));
  }

  SharedMemoryFile::Descriptor descriptor;

  descriptor.setDomain(SharedMemoryFile::Descriptor::DM_FILE);
  descriptor.setOpenMode(SharedMemoryFile::Descriptor::OM_ATTACH);
  descriptor.setName(strFileName);

  ptrSHM.reset(new SharedMemoryFile(descriptor));

  pMemory    = ptrSHM->getAddress();
  iSpaceLeft = ptrSHM->getSize();

  if(!pMemory)
    IA20_THROW(SystemException("Cannot allocate LogFileWriter memory."));

  MetaData *pMetaData = reinterpret_cast<MetaData*>(pMemory);

  if(memcmp(pMetaData->sTag, CTag, CTagLength) != 0 ||
    pMetaData->iFileSize != iSpaceLeft)
    IA20_THROW(BadUsageException("The file is not a preallocated LogFile, name: ")<<strFileName);

  pMetaData->tsStarted.readSystemTimestamp();
  pMetaData->bUsed = true;

  SharedMemoryFile::Sync(pMetaData, sizeof(MetaData));

  pNextEntry  = GetDataStart(pMemory);
  iSpaceLeft -= reinterpret_cast<uint8_t*>(pNextEntry) - reinterpret_cast<uint8_t*>(pMemory);
}
/*************************************************************************/
LogFileWriter::~LogFileWriter() throw(){
	IA20_TRACER;
  ptrSHM->syncAll(true);
}
/*************************************************************************/
const LogEntry* LogFileWriter::appendEntry(const LogEntryId& entryId, LogEntrySizeType  iEntryDataSize, const void* pSrcData){
  IA20_TRACER;

  //TODO configurable asset as this is a double check ...

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: appendEntry: "<<LogEntry::ComputeSpace(iEntryDataSize)<<" ? "<<iSpaceLeft);

  if(LogEntry::ComputeSpace(iEntryDataSize) > iSpaceLeft)
    IA20_THROW(InternalException("LogEntry::ComputeSpace(iEntryDataSize) > iSpaceLeft"));

  LogEntry* pEntry = new (pNextEntry) LogEntry(entryId, pLastEntry, iEntryDataSize, pSrcData);
  pNextEntry = pEntry->next();

  iSpaceLeft -= (reinterpret_cast<uint8_t*>(pNextEntry) - reinterpret_cast<uint8_t*>(pEntry));

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: appendEntry: iSpaceLeft: "<<iSpaceLeft);

  if(!pFirstEntry){
    pFirstEntry = pEntry;
  }

  pLastEntry = pEntry;

  return pEntry;
}
/*************************************************************************/
void LogFileWriter::commit(const LogEntry* pLogEntry){
	IA20_TRACER;

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: commit: "<<*pLogEntry);
  pLastCommit = pLogEntry;
  SharedMemoryFile::Sync(pLogEntry, sizeof(LogEntry) + pLogEntry->getEntryDataSize());
}
/*************************************************************************/
}
}
}
}
