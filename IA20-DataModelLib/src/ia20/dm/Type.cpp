/*
 * File: Type.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "Type.h"
#include "DataObject.h"

namespace IA20 {
namespace DM {

/*************************************************************************/
Type::Type(Kind iKind, const String& strName, const Type* pBaseType):
  iKind(iKind),
  strName(strName),
  pBaseType(pBaseType){
	IA20_TRACER;
}
/*************************************************************************/
Type::~Type() throw(){
	IA20_TRACER;
}
/*************************************************************************/
ComplexType* Type::asComplexType(){
   IA20_THROW(BadUsageException("Not a complex type: ")<<strName);
}
/*************************************************************************/

}
}