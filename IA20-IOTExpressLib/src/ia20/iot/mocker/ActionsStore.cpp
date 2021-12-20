/*
 * File: ActionsStore.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "ActionsStore.h"

namespace IA20 {
namespace IOT {
namespace Mocker {

/*************************************************************************/
ActionsStore::ActionsStore(std::unique_ptr<RingType::Interface>&& ptrInterface):
IOT::ActionsStore(std::move(ptrInterface)){
	IA20_TRACER;
}
/*************************************************************************/
ActionsStore::~ActionsStore() throw(){
	IA20_TRACER;
}
/*************************************************************************/
void ActionsStore::run(){
	IA20_TRACER;

  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(1, &cpuset);
  int rc1 = pthread_setaffinity_np(pthread_self(),sizeof(cpu_set_t), &cpuset);

  SYS::Signal::ThreadRegistration tr;
  Thread::Cancellation ca(true);

  IA20_LOG(false,"ActivityMocker started");

  while(!SYS::Signal::GetInstance()->isStopping()){

    ptrInterface->getResponses()->enque(
        ptrInterface->getRequests()->deque());

    IA20_LOG(false,"ActivityMocker serverd");
  }

  IA20_LOG(false,"ActivityMocker ended");
}
/*************************************************************************/
}
}
}
