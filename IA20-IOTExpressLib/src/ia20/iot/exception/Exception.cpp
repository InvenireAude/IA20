/*
 * File: Exception.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "Exception.h"


#include <ia20/iot/logger/LogLevel.h>

namespace IA20 {
namespace IOT {

/*************************************************************************/
Exception::Exception(){
	IA20_TRACER;
}
/*************************************************************************/
Exception::Exception(const String& strInfo):
  IA20::Exception(strInfo){
	IA20_TRACER;
}
/*************************************************************************/
Exception::~Exception() throw(){
	IA20_TRACER;
}
/*************************************************************************/
const char* Exception::getName(){
	IA20_TRACER;
	return "Exception";
}
/*************************************************************************/
}
}
