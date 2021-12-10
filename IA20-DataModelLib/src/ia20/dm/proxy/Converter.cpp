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

Converter Converter::TheInstance;

/*************************************************************************/
 Type::Integer Converter::convertStringToInteger(const Type::CString csValue){
  try{
		return std::stoi(csValue,0,10);
	}catch(std::exception& e){
    PrintTrace(std::cout);
		IA20_THROW(ConversionException("Conversion error: [")<<csValue<<"]");
	}
 }
/*************************************************************************/
 void Converter::convertIntegerToStream(const Type::Integer iValue, std::ostream& os){
   os<<iValue;
 }
/*************************************************************************/
}
}
}
