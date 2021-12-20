/*
 * File: ActivityStore.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "ActivityStore.h"

namespace IA20 {
namespace IOT {
namespace Mocker {

/*************************************************************************/
ActivityStore::ActivityStore(std::unique_ptr<RingType::Interface>&& ptrInterface):
IOT::ActivityStore(std::move(ptrInterface)){
	IA20_TRACER;
}
/*************************************************************************/
ActivityStore::~ActivityStore() throw(){
	IA20_TRACER;
}
/*************************************************************************/
void ActivityStore::run(){
	IA20_TRACER;
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(2, &cpuset);
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
