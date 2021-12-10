/*
 * File: StringDataObject.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "StringDataObject.h"

#include <ia20/dm/exception/ConversionException.h>
#include <ia20/dm/proxy/Converter.h>
#include <ia20/dm/memory/MemoryManager.h>

#include <string.h>

namespace IA20 {
namespace DM {

/*************************************************************************/
StringDataObject::StringDataObject(const Type* pType, DataObject *pParent):
  SimpleDataObject(pType, pParent){
	IA20_TRACER;
}

/*************************************************************************/
StringDataObject::~StringDataObject() throw(){
	IA20_TRACER;
}
/*************************************************************************/
Type::Integer StringDataObject::getInteger()const{
	IA20_TRACER;
  return Proxy::Converter::TheInstance.convertStringToInteger(mValue.mHolder.csValue);
}
/*************************************************************************/
Type::CString StringDataObject::getCString()const{
	IA20_TRACER;
  return mValue.mHolder.csValue;
}
/*************************************************************************/
void StringDataObject::setString(const String& strValue){
  mValue.mHolder.csValue = (Type::CString)Memory::MemoryManager::AllocateLocally(this, strValue.length() + 1 );
  memcpy((char*)mValue.mHolder.csValue, strValue.c_str(), strValue.length() + 1);
}
/*************************************************************************/
void StringDataObject::saveToStream(std::ostream& os)const{
	IA20_TRACER;
  os<<mValue.mHolder.csValue;
}
/*************************************************************************/
}
}
