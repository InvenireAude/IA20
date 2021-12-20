/*
 * File: ActivityStore.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "ActivityStore.h"


namespace IA20 {
namespace IOT {

/*************************************************************************/
ActivityStore::ActivityStore(std::unique_ptr<RingType::Interface>&& ptrInterface):
ptrInterface(std::move(ptrInterface)){
	IA20_TRACER;
}
/*************************************************************************/
ActivityStore::~ActivityStore() throw(){
	IA20_TRACER;
}
/*************************************************************************/
}
}
