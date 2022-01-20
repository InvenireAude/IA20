/*
 * File: PipeFD.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "PipeFD.h"

#include <fcntl.h>
#include <unistd.h>


namespace IA20 {
namespace IOT {
namespace Tools {
namespace SYS {

/*************************************************************************/
PipeFD::PipeFD(){
	IA20_TRACER;

   int flags = 0;// | O_NONBLOCK;

   if(pipe2(fdRequest,  flags) != 0){
	   IA20_THROW(SystemException("pipe2"));
   };

   if(pipe2(fdResponse,  flags) != 0){
	   IA20_THROW(SystemException("pipe2"));
   };

}
/*************************************************************************/
PipeFD::~PipeFD() throw(){
	// it is responsibility of the user (handler) classes to close these.
}
/*************************************************************************/
}
}
}
}
