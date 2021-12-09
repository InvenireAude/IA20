/*
 * File: StringDataObject.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "StringDataObject.h"

#include <ia20/dm/exception/ConversionException.h>
#include <ia20/dm/proxy/Converter.h>

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
  return Proxy::Converter::TheInstance.convertStringToInteger(mValue.mHolder);
}
/*************************************************************************/
Type::String StringDataObject::getString()const{
	IA20_TRACER;
  IA20_THROW(ConversionException("Unsupported conversion: [")<<mValue.pType->getKind()<<"-> String (const char*) ]");
}
/*************************************************************************/

void StringDataObject::saveToStream(std::ostream& os)const{
	IA20_TRACER;
  Proxy::Converter::TheInstance.convertIntegerToStream(mValue.mHolder, os);
}
/*************************************************************************/
}
}
