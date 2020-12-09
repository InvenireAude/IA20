/*
 * File: Connection.h
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *

 */


#ifndef _IA20_Net_Engine_Raft_Mocker_Connection_H_
#define _IA20_Net_Engine_Raft_Mocker_Connection_H_

#include <ia20/commonlib/commonlib.h>

#include <ia20/net/conn/mcast/Sender.h>
#include <ia20/net/conn/mcast/Receiver.h>

#include "../RaftEngine.h"

#include <list>

namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {
namespace Mocker {

/*************************************************************************/
/** The Connection class.
 *
 */
class Connection :
  public RaftEngine::Sender {
public:

	virtual ~Connection() throw();

  virtual void send(const Packet& packet);
 // virtual bool receive(Packet& packet);

	Connection();

  void add(RaftEngine* pEngine);

protected:

  typedef std::list<RaftEngine*> RaftEnginesList;

  RaftEnginesList lstRaftEngines;

};
/*************************************************************************/
}
}
}
}
}

#endif /* _IA20_Net_Engine_Raft_Mocker_Connection_H_ */
