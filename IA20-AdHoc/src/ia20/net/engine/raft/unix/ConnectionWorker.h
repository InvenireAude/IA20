/*
 * File: ConnectionWorker.h
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *

 */


#ifndef _IA20_Net_Engine_Raft_Unix_ConnectionWorker_H_
#define _IA20_Net_Engine_Raft_Unix_ConnectionWorker_H_

#include <ia20/commonlib/commonlib.h>

#include "../RaftEngine.h"
#include "../Message.h"
#include "Worker.h"

namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {
namespace Unix {

class Connection;

/*************************************************************************/
/** The ConnectionWorker class.
 *
 */
class ConnectionWorker :
  public Worker{
public:

	virtual ~ConnectionWorker() throw();

  virtual void run();

	ConnectionWorker(Connection* pConnection, RaftEngine *pRaftEngine);

protected:
  Connection* pConnection;
};
/*************************************************************************/
}
}
}
}
}

#endif /* _IA20_Net_Engine_Raft_Unix_ConnectionWorker_H_ */
