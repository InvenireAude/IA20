/*
 * File: Listener.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "Listener.h"

#include <ia20/iot/mqtt/Message.h>

namespace IA20 {
namespace IOT {
namespace Mocker {

/*************************************************************************/
Listener::Listener(std::unique_ptr<RingType::Interface>&& ptrInterface):
IOT::Listener(std::move(ptrInterface)){
	IA20_TRACER;
}

/*************************************************************************/
Listener::~Listener() throw(){
	IA20_TRACER;
}
/*************************************************************************/
void Listener::run(){
	IA20_TRACER;

  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(3, &cpuset);
  int rc1 = pthread_setaffinity_np(pthread_self(),sizeof(cpu_set_t), &cpuset);


  SYS::Signal::ThreadRegistration tr;
  Thread::Cancellation ca(true);

  static int icount = 0;
  while(!SYS::Signal::GetInstance()->isStopping()){

    std::unique_ptr<Task> ptrTask(ptrInterface->getResponses()->deque());
    IA20_LOG(false, "Response: "<<*(ptrTask->pMessage)<<" "<<icount++);
    //ptrTask.release();
    //delete ptrTask->pMessage;
  }

}
/*************************************************************************/
void Listener::sendMessage(const String& strHex){
  IA20_TRACER;

  static MQTT::Message *theMessage = NULL;

  std::unique_ptr<Task> ptrTask(new Task);
  if(!theMessage){
    theMessage = new MQTT::Message(strHex);
  }
   ptrTask->pMessage = theMessage;

// {
//   std::unique_ptr<Task> ptrTask(new Task);
//   std::unique_ptr<Task> ptrTask1(new Task);
//   std::unique_ptr<Task> ptrTask2(new Task);
//   std::unique_ptr<Task> ptrTask3(new Task);
//   ptrTask3->pMessage = new MQTT::Message(strHex);
// }
  ptrInterface->getRequests()->enque(ptrTask.release());

}
/*************************************************************************/
}
}
}
