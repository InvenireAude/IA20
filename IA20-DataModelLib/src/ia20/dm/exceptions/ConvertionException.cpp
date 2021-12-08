/*
 * File: ConvertionException.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "ConvertionException.h"


namespace IA20 {
namespace DM {

/*************************************************************************/
ConvertionException::ConvertionException(){
	IA20_TRACER;
}
/*************************************************************************/
ConvertionException::ConvertionException(const String& strInfo):
  DM::Exception(strInfo){
	IA20_TRACER;
}
/*************************************************************************/
ConvertionException::~ConvertionException() throw(){
	IA20_TRACER;
}
/*************************************************************************/
const char* ConvertionException::getName(){
	IA20_TRACER;
	return "ConvertionException";
}
/*************************************************************************/
}
}
