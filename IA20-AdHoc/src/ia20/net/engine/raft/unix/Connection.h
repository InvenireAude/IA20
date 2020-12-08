/*
 * File: Connection.h
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *

 */


#ifndef _IA20_Net_Engine_Raft_Unix_Connection_H_
#define _IA20_Net_Engine_Raft_Unix_Connection_H_

#include <ia20/commonlib/commonlib.h>

#include <ia20/net/conn/mcast/Sender.h>
#include <ia20/net/conn/mcast/Receiver.h>

#include "../RaftEngine.h"

namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {
namespace Unix {

/*************************************************************************/
/** The Connection class.
 *
 */
class Connection :
  public RaftEngine::Sender {
public:

	virtual ~Connection() throw();

  virtual void send(const Packet& packet);
  virtual bool receive(Packet& packet);

	Connection(const String& strInterface, const String& strGroup, uint16_t iGroupPort);

protected:

  std::unique_ptr<Net::Conn::MCast::Sender> ptrSender;
  std::unique_ptr<Net::Conn::MCast::Receiver> ptrReceiver;

};
/*************************************************************************/
}
}
}
}
}

#endif /* _IA20_Net_Engine_Raft_Unix_Connection_H_ */
