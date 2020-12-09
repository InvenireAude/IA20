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

#include <flatbuffers/flatbuffers.h>

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

 inline Packet(uint8_t* pDataStart = 0, void* pCtx = 0):
    pDataStart(pDataStart),
    pCtx(pCtx){
        if(!pCtx)
          PacketFactory::GetInstance()->allocatePacket(*this);
        IA20_LOG(LogLevel::INSTANCE.isDetailedInfo(), "Raft :: Packet("<<pDataStart<<", "<<pCtx<<")");
    }

 inline Packet(const flatbuffers::FlatBufferBuilder& builder):
    pDataStart(NULL),
    pCtx(NULL){
        PacketFactory::GetInstance()->allocatePacket(*this);
        pack(builder);
        IA20_LOG(LogLevel::INSTANCE.isDetailedInfo(), "Raft :: Packet("<<pDataStart<<", "<<pCtx<<")");
    }


    inline Packet(const Packet& other):
          pDataStart(NULL),
          pCtx(NULL){
              this->operator=(other);
    }

  Packet& operator=(const Packet& other){
        PacketFactory::GetInstance()->allocatePacket(*this);
        setLength(other.getLength());
        memcpy(pDataStart, other.pDataStart, getLength());
        IA20_LOG(LogLevel::INSTANCE.isDetailedInfo(), "Raft :: Packet_copy("<<pDataStart<<", "<<pCtx<<")");
        return *this;
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

  inline uint8_t* getDataStart()const{
    return pDataStart;
  }

  inline void* getCtx()const{
    return pCtx;
  }

  inline void release(){
    pCtx = NULL;
  }

  inline void set(uint8_t* pDataStart, void* pCtx){
    if(this->pCtx)
      PacketFactory::GetInstance()->freeCtx(this->pCtx);
    this->pDataStart = pDataStart;
    this->pCtx = pCtx;
    IA20_LOG(LogLevel::INSTANCE.isDetailedInfo(), "Raft :: Packet set("<<pDataStart<<", "<<pCtx<<")");
  }

  void pack(const flatbuffers::FlatBufferBuilder& fb);

protected:

  uint8_t *pDataStart;
  void  *pCtx;

};
/*****************************************************************************/
}
}
}
}

#endif /* _IA20_Net_Engine_Raft_Packet_H_ */
