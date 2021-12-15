/*
 * File: IntegerType.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "IntegerType.h"

#include <ia20/dm/memory/MemoryManager.h>

#include "IntegerDataObject.h"

namespace IA20 {
namespace DM {

const String& IntegerType::CDefaultName("Integer");

/*************************************************************************/
IntegerType::IntegerType(const String& strName, const Type* pBaseType):
  SimpleType(Type::CIntegerType, strName, pBaseType){
	IA20_TRACER;
}
/*************************************************************************/
IntegerType::~IntegerType() throw(){
	IA20_TRACER;
}
/*************************************************************************/
DataObject* IntegerType::create(DataObject *pParent)const{

	IA20_TRACER;

  DataObject *pResult = NULL;

  pResult = Memory::MemoryManager::AllocateLocally<IntegerDataObject>(pParent);
  new(pResult) IntegerDataObject(this, pParent);

  return pResult;
}
/*************************************************************************/
}
}
