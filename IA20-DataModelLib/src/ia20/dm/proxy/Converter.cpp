/*
 * File: Converter.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "Converter.h"

#include <ia20/dm/exceptions/ConvertionException.h>

namespace IA20 {
namespace DM {
namespace Proxy {

/*************************************************************************/
void Converter::ConvertStringToInteger(const TypedValue::Holder& holderFrom, TypedValue::Holder& holderTo){
  holderTo.iValue = atoi(holderFrom.sValue);
}
/*************************************************************************/
void Converter::ConvertByCopy(const TypedValue::Holder& holderFrom, TypedValue::Holder& holderTo){
  holderTo = holderFrom;
}
/*************************************************************************/
#define ConverterTabIdx(i,j) (i + j*Type::Kind::CNumberOfTypeKinds)
/*************************************************************************/
Converter::Converter(){
	IA20_TRACER;

  for(int i=0; i < Type::Kind::CNumberOfTypeKinds; i++)
    for(int j=0; j < Type::Kind::CNumberOfTypeKinds; j++)
      if(i != Type::Kind::CNoneType && i == j){
        tConverters[ConverterTabIdx(i,j)] = &Converter::ConvertByCopy;
    }

  tConverters[ConverterTabIdx(Type::CStringType, Type::CIntegerType)] = &Converter::ConvertStringToInteger;

}
/*************************************************************************/
Converter::~Converter() throw(){
	IA20_TRACER;
}
/*************************************************************************/
void Converter::convert(Type::Kind iKindFrom, const TypedValue::Holder& holderFrom,
                        Type::Kind iKindTo, TypedValue::Holder& holderTo){
	IA20_TRACER;

  ConvertFunPtr ptrFun = tConverters[ConverterTabIdx(iKindFrom, iKindTo)];

  if(!ptrFun){
    IA20_THROW(ConvertionException("Unsupported conversion:[")<<iKindFrom<<","<<iKindTo<<"]");
  }

  (ptrFun)(holderFrom, holderTo);
}
/*************************************************************************/
}
}
}
