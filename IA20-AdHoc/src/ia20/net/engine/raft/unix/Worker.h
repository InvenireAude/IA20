/*
 * File: Worker.h
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *

 */


#ifndef _IA20_Net_Engine_Raft_Unix_Worker_H_
#define _IA20_Net_Engine_Raft_Unix_Worker_H_

#include <ia20/commonlib/commonlib.h>

#include "../RaftEngine.h"
#include "../Message.h"

namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {
namespace Unix {

class Connection;

/*************************************************************************/
/** The Worker class.
 *
 */
class Worker :
  public Thread,
  public Runnable{
public:

	virtual ~Worker() throw();

  virtual void run();

	Worker(Connection* pConnection, RaftEngine *pRaftEngine);

protected:

  Connection* pConnection;
  RaftEngine *pRaftEngine;
};
/*************************************************************************/
}
}
}
}
}

#endif /* _IA20_Net_Engine_Raft_Unix_Worker_H_ */
