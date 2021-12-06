/*
 * File: FileHandler.h
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *

 */


#ifndef _IA20_URing_IO_FileHandler_H_
#define _IA20_URing_IO_FileHandler_H_

#include <ia20/commonlib/commonlib.h>

#include <ia20/uring/EventHandler.h>
#include <ia20/uring/RingHandler.h>
#include <ia20/commonlib/net/conn/tcp/FileHandle.h>
#include <liburing.h>

#include<functional>
#include<tuple>

namespace IA20 {
namespace URing {

namespace IO {

/*************************************************************************/
/** The FileHandler class.
 *
 */
class FileHandler{
public:

protected:

	FileHandler(Net::Conn::TCP::FileHandle* pFileHandle);
  Net::Conn::TCP::FileHandle* pFileHandle;

};

/*************************************************************************/
}
}
}

#endif /* _IA20_URing_IO_FileHandler_H_ */

