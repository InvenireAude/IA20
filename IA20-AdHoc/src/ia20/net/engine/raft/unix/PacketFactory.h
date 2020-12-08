/*
 * File: PacketFactory.h
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *

 */


#ifndef _IA20_Net_Engine_Raft_Unix_PacketFactory_H_
#define _IA20_Net_Engine_Raft_Unix_PacketFactory_H_

#include <ia20/commonlib/commonlib.h>

#include "../PacketFactory.h"
#include "../Packet.h"

namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {
namespace Unix {

/*************************************************************************/
/** The PacketFactory class.
 *
 */
class PacketFactory : public Raft::PacketFactory {
public:

	virtual ~PacketFactory() throw();

	PacketFactory();

  virtual void  allocatePacket(Packet& packet);
  virtual void  freeCtx(void *pCtx);

protected:


};
/*****************************************************************************/
}
}
}
}
}

#endif /* _IA20_Net_Engine_Raft_Unix_PacketFactory_H_ */
