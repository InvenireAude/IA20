/*
 * File: EventHandler.cpp
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *
 */

#include "EventHandler.h"

namespace IA20 {
namespace URing {


/*************************************************************************/
EventHandler::EventHandler(RingHandler* pRingHandler):
  pRingHandler(pRingHandler){
	IA20_TRACER;
}
/*************************************************************************/
EventHandler::~EventHandler() throw(){
	IA20_TRACER;
}
/*************************************************************************/

}
}