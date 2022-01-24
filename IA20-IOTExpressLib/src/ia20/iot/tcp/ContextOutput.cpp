/*
 * File: ContextOutput.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "ContextOutput.h"


#include "ContextOutputDirect.h"
#include "ContextOutputShared.h"

namespace IA20 {
namespace IOT {
namespace TCP {
/*************************************************************************/
ContextOutput::ContextOutput(){
	IA20_TRACER;
}

/*************************************************************************/
ContextOutput::~ContextOutput() throw(){
	IA20_TRACER;
}
/*************************************************************************/
}
}
}
