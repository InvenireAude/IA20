/*
 * File: Packet.h
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *

 */


#ifndef _IA20_Net_Engine_Raft_Packet_H_
#define _IA20_Net_Engine_Raft_Packet_H_

#include <ia20/commonlib/commonlib.h>
#include "Definitions.h"
#include "PacketFactory.h"

namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {

/*************************************************************************/
/** The Packet class.
 *
 */
class Packet {
public:

 typedef uint16_t LengthType;

 inline Packet(void* pDataStart = 0, void* pCtx = 0):
    pDataStart(pDataStart),
    pCtx(pCtx){
        if(!pCtx)
          PacketFactory::GetInstance()->allocatePacket(*this);
        IA20_LOG(LogLevel::INSTANCE.isDetailedInfo(), "Raft :: Packet("<<pDataStart<<", "<<pCtx<<")");
    }

 inline ~Packet(){
    IA20_LOG(LogLevel::INSTANCE.isDetailedInfo(), "Raft :: ~Packet("<<pDataStart<<", "<<pCtx<<")");
    if(pCtx)
      PacketFactory::GetInstance()->freeCtx(pCtx);
  };

  inline void setLength(LengthType iLength){
    LengthType *pLength = reinterpret_cast<LengthType*>(pDataStart) - 1;
    *pLength = iLength;
  }

  inline LengthType getLength()const{
    LengthType *pLength = reinterpret_cast<LengthType*>(pDataStart) - 1;
    return *pLength;
  }

  inline void* getDataStart()const{
    return pDataStart;
  }

  inline void* getCtx()const{
    return pCtx;
  }

  inline void release(){
    pCtx = NULL;
  }

  inline void set(void* pDataStart, void* pCtx){
    if(this->pCtx)
      PacketFactory::GetInstance()->freeCtx(this->pCtx);
    this->pDataStart = pDataStart;
    this->pCtx = pCtx;
    IA20_LOG(LogLevel::INSTANCE.isDetailedInfo(), "Raft :: Packet set("<<pDataStart<<", "<<pCtx<<")");
  }

protected:

  void  *pDataStart;
  void  *pCtx;
};

/*****************************************************************************/
}
}
}
}

#endif /* _IA20_Net_Engine_Raft_Packet_H_ */
