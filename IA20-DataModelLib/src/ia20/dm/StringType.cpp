/*
 * File: StringType.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "StringType.h"
#include <ia20/dm/memory/MemoryManager.h>

#include "StringDataObject.h"

namespace IA20 {
namespace DM {

/*************************************************************************/
StringType::StringType(const String& strName):
SimpleType(Type::CStringType, strName){
	IA20_TRACER;
}

/*************************************************************************/
StringType::~StringType() throw(){
	IA20_TRACER;
}
/*************************************************************************/
DataObject* StringType::create(DataObject *pParent)const{

	IA20_TRACER;

  DataObject *pResult = NULL;

  pResult = Memory::MemoryManager::AllocateLocally<StringDataObject>(pParent);
  new(pResult) StringDataObject(this, pParent);

  return pResult;
}
/*************************************************************************/
}
}
