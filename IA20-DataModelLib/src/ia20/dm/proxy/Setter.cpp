/*
 * File: Setter.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "Setter.h"
#include "Converter.h"
#include "string.h"

#include <ia20/dm/memory/MemoryManager.h>

namespace IA20 {
namespace DM {
namespace Proxy {

Setter TheInstance;

/*************************************************************************/
Setter::Setter(){
	IA20_TRACER;
}
/*************************************************************************/
Setter::~Setter() throw(){
	IA20_TRACER;
}
/*************************************************************************/
void Setter::setInteger(TypedValue* pValue, Type::Integer iValue){
	IA20_TRACER;

  Type::Kind iTargetKind = pValue->getType()->getKind();

  if(iTargetKind == Type::CIntegerType){
    pValue->mHolder.iValue = iValue;
  }else{
    TypedValue::Holder tmpHolder(iValue);
    Converter::TheInstance.convert(Type::CIntegerType, tmpHolder, iTargetKind, pValue->mHolder);
  }

}
/*************************************************************************/
void Setter::setString(TypedValue* pValue, const String& strValue){
	IA20_TRACER;

  Type::Kind iTargetKind = pValue->getType()->getKind();

  if(iTargetKind == Type::CStringType){
    Memory::MemoryManager::SizeType iSize = strValue.length() + 1;
    char *sValue = reinterpret_cast<char*>(Memory::MemoryManager::AllocateLocally(pValue, iSize));
    memcpy(sValue, strValue.c_str(), iSize);
    pValue->mHolder.sValue = sValue;
  }else{
    const TypedValue::Holder tmpHolder(strValue.c_str());
    Converter::TheInstance.convert(Type::CStringType, tmpHolder, iTargetKind, pValue->mHolder);
  }

}
/*************************************************************************/
}
}
}
