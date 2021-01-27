/*
 * File: FileHandler.cpp
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *
 */

#include "FileHandler.h"

#include <ia20/uring/URingException.h>

namespace IA20 {
namespace URing {
namespace IO {

/*************************************************************************/
FileHandler::FileHandler(Net::Conn::TCP::FileHandle* pFileHandle):
  pFileHandle(pFileHandle){}
/*************************************************************************/
}
}
}
