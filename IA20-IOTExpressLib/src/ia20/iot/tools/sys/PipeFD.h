/*
 * File: PipeFD.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_Tools_SYS_PipeFD_H_
#define _IA20_IOT_Tools_SYS_PipeFD_H_

#include <ia20/commonlib/commonlib.h>

namespace IA20 {
namespace IOT {
namespace Tools {
namespace SYS {

/*************************************************************************/
/** The PipeFD class.
 *
 */
class PipeFD {
public:

   typedef std::pair<int,int> PeerType;

    enum Direction {
      D_READ  = 0,
      D_WRITE = 1
    };

    PipeFD();
	  ~PipeFD() throw();
	
   PeerType getServerPeer()const{
     return PeerType(fdRequest[D_READ], fdResponse[D_WRITE]);
   }

   PeerType getClientPeer()const{
     return PeerType(fdResponse[D_READ], fdRequest[D_WRITE]);
   }

protected:
	int fdRequest[2];
    int fdResponse[2];
};

/*************************************************************************/
}
}
}
}

#endif /* _IA20_IOT_Tools_SYS_PipeFD_H_ */
