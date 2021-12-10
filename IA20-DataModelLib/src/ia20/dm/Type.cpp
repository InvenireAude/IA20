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
Type::Type(Kind iKind, const String& strName):
  iKind(iKind),
  strName(strName){
	IA20_TRACER;
}
/*************************************************************************/
Type::~Type() throw(){
	IA20_TRACER;
}
/*************************************************************************/
const ComplexType* Type::asComplexType()const{
   IA20_THROW(BadUsageException("Not a complex type: ")<<strName);
}
/*************************************************************************/

}
}