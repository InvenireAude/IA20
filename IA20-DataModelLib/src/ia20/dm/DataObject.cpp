/*
 * File: DataObject.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "DataObject.h"

#include <ia20/dm/exception/ConversionException.h>

#include <ia20/dm/memory/MemoryManager.h>

#include "ComplexDataObject.h"
#include "IntegerDataObject.h"
#include "StringDataObject.h"

namespace IA20 {
namespace DM {

/*************************************************************************/
DataObject::DataObject(const Type* pType, DataObject *pParent):
  pParent(pParent){
	IA20_TRACER;
  mValue.pType = pType;
  mValue.mHolder.iValue = 0;
}
/*************************************************************************/
DataObject::~DataObject() throw(){
	IA20_TRACER;
}
/*************************************************************************/
DataObject* Create(void *pOwnerAddress, const Type* pType, DataObject *pParent = NULL){
	IA20_TRACER;

  DataObject *pResult = NULL;

  switch(pType->getKind()){ //TODO switch+case => proxy table [getKind()] -> create(pType,pParent);

    case Type::CDataObjectType:
      pResult = Memory::MemoryManager::AllocateLocally<ComplexDataObject>(pOwnerAddress);
      new(pResult) ComplexDataObject(pType, pParent);
    break;

    case Type::CStringType:
      pResult = Memory::MemoryManager::AllocateLocally<StringDataObject>(pOwnerAddress);
      new(pResult) StringDataObject(pType, pParent);
    break;

    case Type::CIntegerType:
      pResult = Memory::MemoryManager::AllocateLocally<StringDataObject>(pOwnerAddress);
      new(pResult) IntegerDataObject(pType, pParent);
    break;

    default:
      IA20_THROW(InternalException("Not implemented Type::Kind in DataObject::Create():")<<pType->getKind());
  }

  return pResult;
}
/*************************************************************************/
DataObject* DataObject::getParent()const{
  IA20_TRACER;
  IA20_CHECK_IF_NULL(pParent);
  return pParent;
}
/*************************************************************************/
Type::Integer DataObject::getInteger()const{
	IA20_TRACER;
  IA20_THROW(ConversionException("Unsupported conversion: [ ")<<mValue.pType->getKind()<<"-> Integer ]");
}
/*************************************************************************/
Type::String DataObject::getString()const{
	IA20_TRACER;
  IA20_THROW(ConversionException("Unsupported conversion: [ ")<<mValue.pType->getKind()<<"-> String ]");
}
/*************************************************************************/
void DataObject::saveToStream(std::ostream& os)const{
	IA20_TRACER;
  IA20_THROW(ConversionException("Unsupported conversion: [ ")<<mValue.pType->getKind()<<"-> StringStream ]");
}
/*************************************************************************/
}
}

