/*
 * File: RaftEngine.h
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *

 */


#ifndef _IA20_Net_Engine_Raft_Definitions_H_
#define _IA20_Net_Engine_Raft_Definitions_H_

#include <ia20/commonlib/commonlib.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>

namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {

  typedef uint32_t IndexType;
  typedef uint32_t TermType;
  typedef uint8_t  ServerIdType;
  typedef uint32_t LogIndexType;
  typedef uint32_t LogEntrySizeType;

  static const int CMaxServers = 255;
  static const int CSeverNull  = 255;

}
}
}
}

#endif /* _IA20_Net_Engine_Raft_Definitions_H_ */
