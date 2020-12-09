/*
 * File: IA20-CommonLib/src/commonlib/memory/sharable/SharedMemoryFile.cpp
 *
 * Copyright (C) 2015, Albert Krzymowski
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/* ChangeLog:
 *
 */

#include "SharedMemoryFile.h"
#include <ia20/commonlib/logger/logger.h>
#include <ia20/commonlib/exception/InternalException.h>
#include <ia20/commonlib/exception/SystemException.h>
#include <ia20/commonlib/tools/TypeTools.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <stdlib.h>

namespace IA20 {
const size_t SharedMemoryFile::CPageSize = getpagesize();
/*************************************************************************/
SharedMemoryFile::Descriptor::Descriptor():
	iDomain(DM_SHARED_MEMORY),
	iOpenMode(OM_CREATE_IF_NEEDED),
	iCloseMode(CM_DEATTACH),
	pAddress(0),
	iSize(0){
	IA20_TRACER;
}
/*************************************************************************/
SharedMemoryFile::Descriptor::Descriptor(const Descriptor& other):
	iDomain(other.iDomain),
	iOpenMode(other.iOpenMode),
	iCloseMode(other.iCloseMode),
	strName(other.strName),
	pAddress(other.pAddress),
	iSize(other.iSize){
	IA20_TRACER;
}
/*************************************************************************/
void SharedMemoryFile::Descriptor::setDomain(Domain iDomain){
	IA20_TRACER;
	this->iDomain=iDomain;
}
/*************************************************************************/
void SharedMemoryFile::Descriptor::setOpenMode(OpenMode iOpenMode){
	IA20_TRACER;
	this->iOpenMode=iOpenMode;
}
/*************************************************************************/
void SharedMemoryFile::Descriptor::setCloseMode(CloseMode iCloseMode){
	IA20_TRACER;
	this->iCloseMode=iCloseMode;
}
/*************************************************************************/
void SharedMemoryFile::Descriptor::setName(const String& strName){
	IA20_TRACER;
	this->strName=strName;
}
/*************************************************************************/
void SharedMemoryFile::Descriptor::setAddress(void *pAddress){
	IA20_TRACER;
	this->pAddress=pAddress;
}
/*************************************************************************/
void SharedMemoryFile::Descriptor::setSize(size_t iSize){
	IA20_TRACER;
	this->iSize=iSize;
}
/*************************************************************************/
SharedMemoryFile::Descriptor::Domain SharedMemoryFile::Descriptor::getDomain() const{
	IA20_TRACER;
	return iDomain;
}
/*************************************************************************/
SharedMemoryFile::Descriptor::OpenMode SharedMemoryFile::Descriptor::getOpenMode() const {
	IA20_TRACER;
	return iOpenMode;
}
/*************************************************************************/
SharedMemoryFile::Descriptor::CloseMode SharedMemoryFile::Descriptor::getCloseMode() const {
	IA20_TRACER;
	return iCloseMode;
}
/*************************************************************************/
const String& SharedMemoryFile::Descriptor::getName() const {
	IA20_TRACER;
	return strName;
}
/*************************************************************************/
void* SharedMemoryFile::Descriptor::getAddress() const {
	IA20_TRACER;
	return pAddress;
}
/*************************************************************************/
size_t SharedMemoryFile::Descriptor::getSize() const {
	IA20_TRACER;
	return iSize;
}
/*************************************************************************/
SharedMemoryFile::SharedMemoryFile(const Descriptor& aDescriptor):
	aDescriptor(aDescriptor),
	bAlreadyExisted(false),
	pAddress(0),
	fd(-1){
	IA20_TRACER;

  switch (aDescriptor.getDomain()){
    case Descriptor::DM_FILE:
      accessFile();
      break;
    case Descriptor::DM_SHARED_MEMORY:
      accessMemory();
      break;
    default:
      IA20_THROW(InternalException("Unknown Shared Memory Domain."));
  }

	mapFile();

}
/*************************************************************************/
void SharedMemoryFile::accessFile() {
	IA20_TRACER;

	IA20_LOG(LogLevel::INSTANCE.isInfo(),"Opening file: ["<<aDescriptor.strName<<"]:"
			<<aDescriptor.iOpenMode);


  int iOpenOptions = 0;

  switch(aDescriptor.iOpenMode){

  case Descriptor::OM_ATTACH:
    iOpenOptions = 0;
    break;

  case Descriptor::OM_CREATE_ALWAYS:
    iOpenOptions = O_TRUNC;

  case Descriptor::OM_CREATE_IF_NEEDED:
    iOpenOptions = O_CREAT /*| O_EXCL*/ ; break;

  default:
    IA20_THROW(InternalException("Unknown mode."));
  }

  if ( (fd = open(aDescriptor.strName.c_str(), iOpenOptions |  O_RDWR, S_IRUSR | S_IWUSR)) == -1)
      IA20_THROW(SystemException("open(): ")<<aDescriptor.strName);

	struct stat info;

	IA20_LOG(LogLevel::INSTANCE.isInfo(),"Opening file: "<<aDescriptor.strName<<",fd = "<<fd);

	if(fstat(fd, &info) < 0)
		IA20_THROW(SystemException("fstat(): ")<<aDescriptor.strName<<", fd:"<<TypeTools::IntToString(fd));

	bAlreadyExisted = info.st_size > 0;

	if(bAlreadyExisted && aDescriptor.getSize() == 0)
		aDescriptor.setSize(info.st_size);

	IA20_LOG(LogLevel::INSTANCE.isInfo(),"Attached file: "<<info.st_size<<" bytes.");

}
/*************************************************************************/
void SharedMemoryFile::accessMemory() {
	IA20_TRACER;

	IA20_LOG(LogLevel::INSTANCE.isInfo(),"Opening memory: ["<<aDescriptor.strName<<"]:"
			<<aDescriptor.iOpenMode);

  int iOpenOptions = 0;

  switch(aDescriptor.iOpenMode){

  case Descriptor::OM_ATTACH           : iOpenOptions = 0; break;

  case Descriptor::OM_CREATE_ALWAYS :
    shm_unlink(aDescriptor.strName.c_str());

  case Descriptor::OM_CREATE_IF_NEEDED    : iOpenOptions = O_CREAT /*| O_EXCL*/ ; break;

  default:
    IA20_THROW(InternalException("Unknown mode."));
  }

  if ((fd=shm_open(aDescriptor.strName.c_str(),
        iOpenOptions |  O_RDWR,
        S_IRUSR | S_IWUSR)) == -1)
    IA20_THROW(SystemException(aDescriptor.strName+" -> shm_open"));

	struct stat info;

	IA20_LOG(LogLevel::INSTANCE.isInfo(),"Opening memory: "<<aDescriptor.strName<<",fd = "<<fd);


	if(fstat(fd, &info) < 0)
		IA20_THROW(SystemException(aDescriptor.strName+" -> fstat" +TypeTools::IntToString(fd)));

	bAlreadyExisted = info.st_size > 0;

	if(bAlreadyExisted && aDescriptor.getSize() == 0)
		aDescriptor.setSize(info.st_size);

	IA20_LOG(LogLevel::INSTANCE.isInfo(),"Attached: "<<info.st_size<<" bytes.");

}
/*************************************************************************/
void SharedMemoryFile::mapFile() {
	IA20_TRACER;

	if(!bAlreadyExisted){
		IA20_LOG(LogLevel::INSTANCE.isInfo(),"Truncate to: "<<aDescriptor.iSize<<" bytes.");
		if(ftruncate(fd,aDescriptor.iSize)< 0)
			IA20_THROW(SystemException(aDescriptor.strName+" -> ftruncate"));
	}

	IA20_LOG(LogLevel::INSTANCE.isInfo(),"Attaching at: "<<(void*)aDescriptor.pAddress<<", "<<
			aDescriptor.iSize<<" bytes.");

	pAddress = mmap(aDescriptor.pAddress,
				    aDescriptor.iSize,
					PROT_READ | PROT_WRITE,
					(aDescriptor.pAddress==0) ? MAP_SHARED : MAP_FIXED | MAP_SHARED,
					fd,
					0);

	if(pAddress == NULL || pAddress == MAP_FAILED){
		IA20_THROW(SystemException(aDescriptor.strName+" -> mmap "));
	}

	IA20_LOG(LogLevel::INSTANCE.isSystem(),aDescriptor.strName<<" attached at: "<<pAddress);

}
/*************************************************************************/
SharedMemoryFile::~SharedMemoryFile() throw () {
	IA20_TRACER;

	IA20_LOG(LogLevel::INSTANCE.isInfo(),"Cleanup of SharedMemoryFile: ["<<aDescriptor.strName<<"]");

	if(pAddress)
		munmap(pAddress,aDescriptor.iSize);

	if(fd >= 0)
		close(fd);

	if(aDescriptor.iDomain == Descriptor::DM_SHARED_MEMORY &&
		aDescriptor.iCloseMode == Descriptor::CM_DESTROY){
		IA20_LOG(LogLevel::INSTANCE.isInfo(),"Remove object: ["<<aDescriptor.strName<<"]");
		shm_unlink(aDescriptor.strName.c_str());
	}
}
/*************************************************************************/
size_t SharedMemoryFile::getSize() const{
	IA20_TRACER;
	return aDescriptor.iSize;
}
/*************************************************************************/
bool SharedMemoryFile::alreadyExisted() const{
	IA20_TRACER;
	return bAlreadyExisted;
}
/*************************************************************************/
void* SharedMemoryFile::getAddress() const{
	IA20_TRACER;
	if(!pAddress)
		IA20_THROW(InternalException("Null address in SharedMemoryFile::getAddress()."));
	return pAddress;
}
/*************************************************************************/
const SharedMemoryFile::Descriptor& SharedMemoryFile::getDescriptor() const{
	IA20_TRACER;
	return aDescriptor;
}
/*************************************************************************/
void SharedMemoryFile::Sync(const void* pAddress, size_t iDataLength, bool bAsync){
	IA20_TRACER;

  int iFlags = bAsync ? MS_ASYNC : MS_SYNC;
  iFlags |= MS_INVALIDATE;

	IA20_LOG(LogLevel::INSTANCE.isSystem()," MSync[1]: p: "<<pAddress<<", sz: "<<iDataLength);

  const uint8_t* pAddressPtr = reinterpret_cast<const uint8_t*>(pAddress);
  const uint8_t* pAlignedPtr = reinterpret_cast<const uint8_t*>((uint64_t)pAddressPtr & ~(CPageSize - 1));

  iDataLength += pAddressPtr - pAlignedPtr;

 	IA20_LOG(LogLevel::INSTANCE.isSystem()," MSync[2]: p: "<<(void*)pAlignedPtr<<", sz: "<<iDataLength);

  if(msync(const_cast<uint8_t*>(pAlignedPtr), iDataLength, iFlags) == -1)
    IA20_THROW(SystemException("SharedMemoryFile::Sync, sync() failed."));

}
/*************************************************************************/
void SharedMemoryFile::syncAll(bool bAsync){
	IA20_TRACER;
  Sync(pAddress, getSize(), bAsync);
}
/*************************************************************************/

}/* namespace IA20 */
