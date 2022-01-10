/*
 * File: InternalException.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "InternalException.h"


#include <ia20/iot/logger/LogLevel.h>

namespace IA20 {
namespace IOT {

/*************************************************************************/
InternalException::InternalException(){
	IA20_TRACER;
}
/*************************************************************************/
InternalException::InternalException(const String& strInfo):
  IOT::Exception(strInfo){
	IA20_TRACER;
}
/*************************************************************************/
InternalException::~InternalException() throw(){
	IA20_TRACER;
}
/*************************************************************************/
const char* InternalException::getName(){
	IA20_TRACER;
	return "InternalException";
}
/*************************************************************************/
}
}
