/*
 * File: Converter.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "Converter.h"

#include <ia20/dm/exception/ConversionException.h>

namespace IA20 {
namespace DM {
namespace Proxy {

/*************************************************************************/
 Type::Integer convertStringToInteger(const TypedValue::Holder& holderFrom){
  try{
		return std::stoi(holderFrom.sValue,0,10);
	}catch(std::exception& e){
		IA20_THROW(ConversionException("Conversion error: [")<<holderFrom.sValue<<"]");
	}
 }
/*************************************************************************/
 void convertIntegerToStream(const TypedValue::Holder& holderFrom, std::ostream& os){
   os<<holderFrom.iValue;
 }
/*************************************************************************/
}
}
}
