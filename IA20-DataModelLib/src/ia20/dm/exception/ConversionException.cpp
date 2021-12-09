/*
 * File: ConversionException.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "ConversionException.h"


namespace IA20 {
namespace DM {

/*************************************************************************/
ConversionException::ConversionException(){
	IA20_TRACER;
}
/*************************************************************************/
ConversionException::ConversionException(const String& strInfo):
  DM::Exception(strInfo){
	IA20_TRACER;
}
/*************************************************************************/
ConversionException::~ConversionException() throw(){
	IA20_TRACER;
}
/*************************************************************************/
const char* ConversionException::getName(){
	IA20_TRACER;
	return "ConversionException";
}
/*************************************************************************/
}
}
