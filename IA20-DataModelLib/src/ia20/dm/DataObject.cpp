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
#include "Type.h"
#include "ComplexType.h"
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
DataObject* DataObject::getParent()const{
  IA20_TRACER;
  IA20_CHECK_IF_NULL(pParent);
  return pParent;
}
/*************************************************************************/
Type::Integer DataObject::getInteger()const{
	IA20_TRACER;
  IA20_THROW(ConversionException("Unsupported conversion: [ ")<<mValue.pType->getName()<<"-> Integer ]");
}
/*************************************************************************/
Type::CString DataObject::getCString()const{
	IA20_TRACER;
  IA20_THROW(ConversionException("Unsupported conversion: [ ")<<mValue.pType->getName()<<"-> CString ]");
}
/*************************************************************************/
void DataObject::setInteger(Type::Integer iValue){
	IA20_TRACER;
  IA20_THROW(ConversionException("Unsupported conversion: [ ")<<mValue.pType->getName()<<"<= Integer ]");
}
/*************************************************************************/
void DataObject::setString(const String& strValue){
	IA20_TRACER;
  IA20_THROW(ConversionException("Unsupported conversion: [ ")<<mValue.pType->getName()<<"<- CString ]");
}
/*************************************************************************/
void DataObject::saveToStream(std::ostream& os)const{
	IA20_TRACER;
  IA20_THROW(ConversionException("Unsupported conversion: [ ")<<mValue.pType->getName()<<"-> StringStream ]");
}
/*************************************************************************/
void DataObject::createProperty(unsigned int iIdx){
  IA20_TRACER;
  IA20_THROW(ConversionException("Create property not allowed for: ")<<mValue.pType->getName());
}
/*************************************************************************/
void DataObject::setProperty(unsigned int iIdx, DataObject* pDataObject){
  IA20_TRACER;
  IA20_THROW(ConversionException("Set property not allowed for: ")<<mValue.pType->getName());
}
/*************************************************************************/
void DataObject::setProperty(const String& strName, DataObject* pDataObject){
  IA20_TRACER;
  IA20_THROW(ConversionException("Set property not allowed for: ")<<mValue.pType->getName());
}
/*************************************************************************/
DataObject* DataObject::getProperty(unsigned int iIdx)const{
  IA20_TRACER;
  IA20_THROW(ConversionException("Get property not allowed for: ")<<mValue.pType->getName());
}
/*************************************************************************/
DataObject* DataObject::getProperty(const String& strName)const{
  IA20_TRACER;
  IA20_THROW(ConversionException("Get property not allowed for: ")<<mValue.pType->getName());
}
/*************************************************************************/

}
}

