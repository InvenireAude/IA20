/*
 * File: RingHandler.cpp
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *
 */

#include "RingHandler.h"

#include "URingException.h"

#include <strings.h>

namespace IA20 {
namespace URing {

/*************************************************************************/
RingHandler::RingHandler(){
	IA20_TRACER;

  struct io_uring_params params;

  bzero(&params, sizeof(params));
  //params.flags |= IORING_SETUP_IOPOLL;
 // params.flags |= IORING_SETUP_SQPOLL;
 // params.sq_thread_idle = 10000;


  int iResult = io_uring_queue_init_params(CDefaultSize, &ring, &params);

  if(iResult != 0)
    IA20_THROW(URingException("io_uring_queue_init", -iResult));

}
/*************************************************************************/
RingHandler::~RingHandler() throw(){
	IA20_TRACER;

  io_uring_queue_exit(&ring);

}
/*************************************************************************/
void RingHandler::onEvent(struct io_uring_cqe *cqe){
	IA20_TRACER;

  IA20_CHECK_IF_NULL(cqe);
  IA20_CHECK_IF_NULL(cqe->user_data);

  IA20_LOG(LogLevel::INSTANCE.isSystem(), "Got event: res="<<cqe->res);

  reinterpret_cast<EventHandler*>(cqe->user_data)->handle(cqe->res);

}
/*************************************************************************/
static inline void _submit_and_check(struct io_uring* pRing){

    int iResult = io_uring_submit(pRing);

    if(iResult < 0)
        IA20_THROW(URingException("io_uring_submit", -iResult));
}
/*************************************************************************/
void RingHandler::prepareRead(EventHandler* pEventHandler, int fd, struct iovec* iovec, off_t iOffset){
  IA20_TRACER;

  IA20_LOG(LogLevel::INSTANCE.isSystem(), "Prepare read, fd: "<<fd<<" "<<(void*)this);

  io_uring_sqe *sqe = io_uring_get_sqe(&ring);

  if(!sqe)
    IA20_THROW(URingException("io_uring_get_sqe",0));

  io_uring_prep_readv(sqe, fd, iovec, 1, 0);

  io_uring_sqe_set_data(sqe, pEventHandler);
  //_submit_and_check(&ring);
}
/*************************************************************************/
void RingHandler::prepareClose(EventHandler* pEventHandler, int fd){
  IA20_TRACER;

  IA20_LOG(LogLevel::INSTANCE.isSystem(), "Prepare close, fd: "<<fd);

  io_uring_sqe *sqe = io_uring_get_sqe(&ring);

  if(!sqe)
    IA20_THROW(URingException("io_uring_get_sqe",0));

  io_uring_prep_close(sqe, fd);

  io_uring_sqe_set_data(sqe, pEventHandler);
  //_submit_and_check(&ring);
}
/*************************************************************************/
void RingHandler::prepareShutdown(EventHandler* pEventHandler, int fd, int how){
  IA20_TRACER;

  IA20_LOG(LogLevel::INSTANCE.isSystem(), "Prepare shutdown, fd: "<<fd);

  io_uring_sqe *sqe = io_uring_get_sqe(&ring);

  if(!sqe)
    IA20_THROW(URingException("io_uring_get_sqe",0));

  io_uring_prep_shutdown(sqe, fd, how);

  io_uring_sqe_set_data(sqe, pEventHandler);
  //_submit_and_check(&ring);
}
/*************************************************************************/
void RingHandler::prepareWrite(EventHandler* pEventHandler, int fd, struct iovec* iovec, off_t iOffset){
  IA20_TRACER;

  IA20_LOG(LogLevel::INSTANCE.isSystem(), "Prepare write, fd: "<<fd<<" "<<(void*)this);

  io_uring_sqe *sqe = io_uring_get_sqe(&ring);

  if(!sqe)
    IA20_THROW(URingException("io_uring_get_sqe",0));

  io_uring_prep_writev(sqe, fd, iovec, 1, 0);

  io_uring_sqe_set_data(sqe, pEventHandler);
  //_submit_and_check(&ring);
}
/*************************************************************************/
void RingHandler::prepareAccept(EventHandler* pEventHandler, int fd, Net::Conn::Address& address, int flags){
 	IA20_TRACER;

  IA20_LOG(LogLevel::INSTANCE.isSystem(), "Prepare accept");

  io_uring_sqe *sqe = io_uring_get_sqe(&ring);

  if(!sqe)
    IA20_THROW(URingException("io_uring_get_sqe",0));

  io_uring_prep_accept(sqe, fd,
          (struct sockaddr *)&address.address, &address.iAddressLen, 0);

  io_uring_sqe_set_data(sqe, pEventHandler);
 // _submit_and_check(&ring);
}
//TODO static inline non-member fun ?
/*************************************************************************/
  void RingHandler::handle(){
    IA20_TRACER;

    struct io_uring_cqe *cqe;
    int iResult = -62;

    // IA20_LOG(LogLevel::INSTANCE.isSystem(), "Waiting for ring SQE ...");

   _submit_and_check(&ring);

   //while(iResult == -62){ // wait is not a cancellation point
      Thread::Cancellation tc(true);
      __kernel_timespec ts = { 1L , 0L };
    //  iResult = io_uring_wait_cqe(&ring, &cqe);
      //iResult = io_uring_wait_cqe_timeout(&ring, &cqe, &ts);
      //usleep(1000000);
      // iResult = 1;

     for(int i=0; i<1000000 && iResult !=0; i++)
        iResult = io_uring_wait_cqe_nr(&ring, &cqe, 0);

      // if(iResult != 0){
      //   iResult = io_uring_wait_cqe_timeout(&ring, &cqe, &ts);
      // }
  //}
// =======
//       //usleep(1);
//       iResult = 1;

//       for(int i=0; i<100000 && iResult !=0; i++)
//         iResult =  io_uring_wait_cqe_nr(&ring, &cqe, 0);

//       if(iResult != 0)
//         iResult = io_uring_wait_cqe_timeout(&ring, &cqe, &ts);
// >>>>>>> c392d7715f0a65ca912543967e35f8f77e3a4d42

     // Thread::Cancellation::Test();
    //  IA20_LOG(LogLevel::INSTANCE.isSystem(), "iResult = "<<iResult);
    //}

    if(iResult < 0){
        //IA20_THROW(URingException("io_uring_wait_cqe", -iResult));
    }else{
      try{
        onEvent(cqe);
      }catch(IA20::Exception& e){
        e.printToStream(std::cerr);
      }
    }

    io_uring_cqe_seen(&ring, cqe);

  // TODO if necessary
    ////_submit_and_check(&ring);

}
/*************************************************************************/
  void RingHandler::handleAtLeastOnce(){
    IA20_TRACER;

    struct io_uring_cqe *cqe;
    int iResult = -62;

    // IA20_LOG(LogLevel::INSTANCE.isSystem(), "Waiting for ring SQE ...");

   _submit_and_check(&ring);

   while(iResult == -62){ // wait is not a cancellation point
      Thread::Cancellation tc(true);
      __kernel_timespec ts = { 1L , 0L };
    //  iResult = io_uring_wait_cqe(&ring, &cqe);
      iResult = io_uring_wait_cqe_timeout(&ring, &cqe, &ts);
      //usleep(1000000);
      // iResult = 1;

    //  for(int i=0; i<1000000 && iResult !=0; i++)
    //     iResult = io_uring_wait_cqe_nr(&ring, &cqe, 1);

      // if(iResult != 0){
      //   iResult = io_uring_wait_cqe_timeout(&ring, &cqe, &ts);
     }
  //}
// =======
//       //usleep(1);
//       iResult = 1;

//       for(int i=0; i<100000 && iResult !=0; i++)
//         iResult =  io_uring_wait_cqe_nr(&ring, &cqe, 0);

//       if(iResult != 0)
//         iResult = io_uring_wait_cqe_timeout(&ring, &cqe, &ts);
// >>>>>>> c392d7715f0a65ca912543967e35f8f77e3a4d42

     // Thread::Cancellation::Test();
    //  IA20_LOG(LogLevel::INSTANCE.isSystem(), "iResult = "<<iResult);
    //}

    if(iResult < 0){
        //IA20_THROW(URingException("io_uring_wait_cqe", -iResult));
    }else{
      try{
        onEvent(cqe);
      }catch(IA20::Exception& e){
        e.printToStream(std::cerr);
      }
    }

    io_uring_cqe_seen(&ring, cqe);

  // TODO if necessary
    ////_submit_and_check(&ring);

}
/*************************************************************************/
void RingHandler::loop(){
  IA20_TRACER;

  while(!SYS::Signal::GetInstance()->isStopping()){
    handle();
  };

}
/*************************************************************************/
}
}
