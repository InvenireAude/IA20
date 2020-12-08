/*
 * File: Packet.cpp
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *
 */

#include "Packet.h"

namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {

/*************************************************************************/
void Packet::pack(const flatbuffers::FlatBufferBuilder& fb){
  IA20_TRACER;

  uint32_t iDataSize         = fb.GetSize();
  void*    pFlatBufferStart  = fb.GetBufferPointer();

  if(iDataSize){
    memcpy(pDataStart, pFlatBufferStart, iDataSize);
  }

  setLength(iDataSize);
}
/*************************************************************************/
}
}
}
}
