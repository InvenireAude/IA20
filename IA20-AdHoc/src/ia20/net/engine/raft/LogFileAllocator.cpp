/*
 * File: LogFileAllocator.cpp
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *
 */

#include "LogFileAllocator.h"

#include "LogEntry.h"

namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {

/*************************************************************************/
LogFileAllocator::LogFileAllocator(const String& strFileName, size_t iSize):
  LogFile(strFileName){
	IA20_TRACER;

  SharedMemoryFile::Descriptor descriptor;

  descriptor.setDomain(SharedMemoryFile::Descriptor::DM_FILE);
  descriptor.setOpenMode(SharedMemoryFile::Descriptor::OM_CREATE_ALWAYS);
  descriptor.setSize(iSize);
  descriptor.setName(strFileName);

  //TODO pre allocate log file ....

  ptrSHM.reset(new SharedMemoryFile(descriptor));

  void* pMemory    = ptrSHM->getAddress();
  size_t iSpaceLeft = ptrSHM->getSize();

  if(!pMemory)
    IA20_THROW(SystemException("Cannot allocate LogFileAllocator memory."));

  bzero(pMemory, iSpaceLeft);

  MetaData *pMetaData = reinterpret_cast<MetaData*>(pMemory);

  memcpy(pMetaData->sTag, CTag, CTagLength);
  pMetaData->iFileSize = iSpaceLeft;
  pMetaData->tsStarted.readSystemTimestamp();
  pMetaData->bUsed = false;
  ptrSHM->syncAll(true);
}
/*************************************************************************/
LogFileAllocator::~LogFileAllocator() throw(){
	IA20_TRACER;
}
/*************************************************************************/
}
}
}
}
