/*
 * File: ActionsStore.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "ActionsStore.h"


namespace IA20 {
namespace IOT {

/*************************************************************************/
ActionsStore::ActionsStore(std::unique_ptr<RingType::Interface>&& ptrInterface):
ptrInterface(std::move(ptrInterface)){
	IA20_TRACER;
}
/*************************************************************************/
ActionsStore::~ActionsStore() throw(){
	IA20_TRACER;
}
/*************************************************************************/
}
}
