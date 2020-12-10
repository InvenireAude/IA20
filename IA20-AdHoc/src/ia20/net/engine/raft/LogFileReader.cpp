/*
 * File: LogFileReader.cpp
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *
 */

#include "LogFileReader.h"

#include "LogEntry.h"

namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {

/*************************************************************************/
LogFileReader::LogFileReader(const String& strFileName):
  LogFile(strFileName){
	IA20_TRACER;

  SharedMemoryFile::Descriptor descriptor;

  descriptor.setDomain(SharedMemoryFile::Descriptor::DM_FILE);
  descriptor.setOpenMode(SharedMemoryFile::Descriptor::OM_ATTACH);
  descriptor.setName(strFileName);

  ptrSHM.reset(new SharedMemoryFile(descriptor));

  pMemory    = ptrSHM->getAddress();
  iSize      = ptrSHM->getSize();

  if(!pMemory)
    IA20_THROW(SystemException("Cannot allocate LogFileReader memory."));

  pMetaData = reinterpret_cast<MetaData*>(pMemory);

  if(memcmp(pMetaData->sTag, CTag, CTagLength) != 0){
    IA20_LOG(true,MiscTools::BinarytoHex(pMetaData, sizeof(MetaData)));
    IA20_THROW(BadUsageException("The file is not a preallocated LogFile, name: ")<<strFileName);
  }

}
/*************************************************************************/
LogFileReader::~LogFileReader() throw(){
	IA20_TRACER;
}
/*************************************************************************/
static inline size_t _distance(const LogEntry* pEntries, const LogEntry* pCurrent){
  return reinterpret_cast<const uint8_t*>(pCurrent) - reinterpret_cast<const uint8_t*>(pEntries);
}
/*************************************************************************/
void LogFileReader::dump(std::ostream& os)const{
  IA20_TRACER;

  os<<" Log file:  "<<strFileName<<std::endl;
  os<<" Used:      "<<(pMetaData->bUsed ? "yes" : "no")<<std::endl;
  os<<" Timestamp: "<<pMetaData->tsStarted.toString()<<std::endl;
  os<<std::endl;

  uint32_t iEntryIdx = 0;

  const LogEntry* pEntries = reinterpret_cast<const LogEntry*>(GetDataStart(pMemory));
  const LogEntry* pCurrent = pEntries;

  const uint32_t iEntriesDataSize = iSize - (reinterpret_cast<const uint8_t*>(pEntries) - reinterpret_cast<const uint8_t*>(pMemory));

  while(_distance(pEntries, pCurrent) < iEntriesDataSize &&
        pCurrent->isValid()){

    const LogEntryId& entryId(pCurrent->getEntryId());
    os<<"["<<entryId.iTerm<<","<<entryId.iIndex<<"]";

    os<<"\t C:"<<pCurrent->isCommited();
    os<<"\t S:"<<pCurrent->getEntryDataSize();
    os<<"\t O:"<<pCurrent->getPrevOffset();

    uint32_t iCheckSum = pCurrent->getCheckSum();

    os<<"\t CS:"<<MiscTools::BinarytoHex(&iCheckSum, sizeof(uint32_t));
    if(pCurrent->getEntryDataSize()){
      //os<<", D: "<<MiscTools::BinarytoHex(pCurrent->getData(), pCurrent->getEntryDataSize());
      String strData;

      for(int i=0;i<pCurrent->getEntryDataSize();i++)
			  strData += String("") + (*(((char*)pCurrent->getData())+i) >= ' ' ? *(((char*)pCurrent->getData())+i) : '.');

      os<<"\t D: "<<strData;
    }

    os<<std::endl;

    pCurrent = pCurrent->next();
  }
}
/*************************************************************************/
}
}
}
}
