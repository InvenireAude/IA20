/*
 * File: IntegerDataObject.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "IntegerDataObject.h"

#include <ia20/dm/exception/ConversionException.h>
#include <ia20/dm/proxy/Converter.h>

namespace IA20 {
namespace DM {

/*************************************************************************/
IntegerDataObject::IntegerDataObject(const Type* pType, DataObject *pParent):
  SimpleDataObject(pType, pParent){
	IA20_TRACER;
}

/*************************************************************************/
IntegerDataObject::~IntegerDataObject() throw(){
	IA20_TRACER;
}
/*************************************************************************/
Type::Integer IntegerDataObject::getInteger()const{
	IA20_TRACER;
  return mValue.mHolder.iValue;
}
/*************************************************************************/
void IntegerDataObject::saveToStream(std::ostream& os)const{
	IA20_TRACER;
  Proxy::Converter::TheInstance.convertIntegerToStream(mValue.mHolder, os);
}
/*************************************************************************/

}
}
