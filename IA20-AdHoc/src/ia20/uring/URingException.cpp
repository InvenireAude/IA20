/*
 * File: URingException.cpp
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *
 */

#include "URingException.h"


namespace IA20 {
namespace URing {

/*************************************************************************/
URingException::URingException(const String& strInfo, int iErrno):
  SystemException(strInfo, iErrno){
	IA20_TRACER;
}

/*************************************************************************/
URingException::~URingException() throw(){
	IA20_TRACER;
}
/*************************************************************************/
const char* URingException::getName(){
	IA20_TRACER;
	return "URingException";
}
/*************************************************************************/
}
}
