/*
 * File: Engine.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "Engine.h"

#include "mqtt/Message.h"

namespace IA20 {
namespace IOT {

/*************************************************************************/
Engine::Engine(
    Listener::RingType::Interface*      pListenerInterface,
    ActivityStore::RingType::Interface* pActivityStoreInterface,
    ActionsStore::RingType::Interface*  pActionsStoreInterface):
  pListenerInterface(pListenerInterface),
  pActivityStoreInterface(pActivityStoreInterface),
  pActionsStoreInterface(pActionsStoreInterface){
	IA20_TRACER;

  pActionsStoreRingRequest = pActionsStoreInterface->getRequests();
  pActionsStoreRingResponse = pActionsStoreInterface->getResponses();

  pActivityStoreRingRequest  = pActivityStoreInterface->getRequests();
  pActivityStoreRingResponse = pActivityStoreInterface->getResponses();

  pListenerRingRequest  = pListenerInterface->getRequests();
  pListenerRingResponse = pListenerInterface->getResponses();

}

/*************************************************************************/
Engine::~Engine() throw(){
	IA20_TRACER;
}
/*************************************************************************/
void Engine::serveListener(){
  IA20_TRACER;

  IA20_LOG(false, "Serve Listener ");

  std::unique_ptr<Listener::Task> ptrTask(pListenerRingRequest->deque());

  MQTT::Message& m = *ptrTask->pMessage;

  IA20_LOG(false, "Got1: "<<m);

  std::unique_ptr<ActivityStore::Task> ptrTask2(new ActivityStore::Task);
  ptrTask2->pMessage = ptrTask->pMessage;
  pActivityStoreRingRequest->enque(ptrTask2.release());
}
/*************************************************************************/
void Engine::serveActivityStore(){
  IA20_TRACER;

  IA20_LOG(false, "Serve ActivityStore ");

  std::unique_ptr<ActivityStore::Task> ptrTask(pActivityStoreRingResponse->deque());

  MQTT::Message& m = *ptrTask->pMessage;

  IA20_LOG(false, "Got2: "<<m);

  std::unique_ptr<ActionsStore::Task> ptrTask2(new ActionsStore::Task);
  ptrTask2->pMessage = ptrTask->pMessage;
  pActionsStoreRingResponse->enque(ptrTask2.release());
}
/*************************************************************************/
void Engine::serveActionsStore(){
  IA20_TRACER;

  IA20_LOG(false, "Serve ActionsStore ");

  std::unique_ptr<ActionsStore::Task> ptrTask(pActionsStoreRingResponse->deque());

  MQTT::Message& m = *ptrTask->pMessage;

  IA20_LOG(false, "Got3: "<<m);

  std::unique_ptr<Listener::Task> ptrTask2(new Listener::Task);
  ptrTask2->pMessage = ptrTask->pMessage;
  pListenerRingResponse->enque(ptrTask2.release());

}
/*************************************************************************/
}
}
