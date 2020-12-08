/*
 * File: PacketFactory.h
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *

 */


#ifndef _IA20_Net_Engine_Raft_PacketFactory_H_
#define _IA20_Net_Engine_Raft_PacketFactory_H_

#include <ia20/commonlib/commonlib.h>

namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {

class Packet;

/*************************************************************************/
/** The PacketFactory class.
 *
 */
class PacketFactory {
  public:

    virtual void allocatePacket(Packet& packet)  = 0;
    virtual void freeCtx(void *pCtx) = 0;


    static const uint16_t CMaxPacketSize = 1500;

    inline static PacketFactory* GetInstance(){
      IA20_CHECK_IF_NULL(ThePacketFactory);
      return ThePacketFactory;
    }

    static inline void SetInstance(PacketFactory* pPacketFactory){
      ThePacketFactory = pPacketFactory;
    }

  protected:

  static thread_local PacketFactory *ThePacketFactory;

/*****************************************************************************/
};

}
}
}
}

#endif /* _IA20_Net_Engine_Raft_PacketFactory_H_ */
