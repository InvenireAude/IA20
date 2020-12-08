/*
 * File: Message.h
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *

 */


#ifndef _IA20_Net_Engine_Raft_Message_H_
#define _IA20_Net_Engine_Raft_Message_H_

#include <ia20/commonlib/commonlib.h>
#include "Definitions.h"

#include "Packet.h"

namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {

class VoteRequestMessage;
class VoteResponseMessage;
class AppendEntriesRequest;
class AppendEntriesResponse;

/*************************************************************************/
/** The Message class.
 *
 */
class Message {


public:

  enum Type: uint16_t{

    MT_None = 0,
    MT_VoteRequest  = 1,
    MT_VoteResponse = 2,
    MT_AppendEntriesRequest  = 11,
    MT_AppendEntriesResponse = 12

  };

  static Type GetType(const Packet& packet){
      const Header *pHeader = reinterpret_cast<Header*>(packet.getDataStart());
      return pHeader->iType;
  }

protected:

  struct Header{
    Type            iType;
    ServerIdType    iDstServerId;
  };

public:

  void setType(Type iType){
    pHeader->iType = iType;
  }

  Type getType()const{
    return pHeader->iType;
  }

  void setDstServer(ServerIdType  iDstServerId){
    pHeader->iDstServerId = iDstServerId;
  }

  ServerIdType getDstServer()const{
    return pHeader->iDstServerId;
  }

protected:

 inline Message(Packet& packet):
   pHeader(reinterpret_cast<Header*>(packet.getDataStart())),
   packet(packet){}

  void setLength(uint16_t iLength)const{
    packet.setLength(iLength);
  }

  Packet& packet;
  Header *pHeader;
};

/*****************************************************************************/

class VoteRequestMessage : public Message {
  public:
   inline VoteRequestMessage(Packet& packet):
      Message(packet){
        pHeader->iType = MT_VoteRequest;
        setLength(sizeof(Header) + sizeof(Request));
      };

  struct Request {
    TermType       iTerm;
    ServerIdType   iCandidate;
    TermType       iLastLogTerm;
    LogIndexType   iLastLogIndexId;
  };

  Request* request()const{
    return  reinterpret_cast<Request*>(pHeader + 1);
  };

};

/*****************************************************************************/
class VoteResponseMessage : public Message {
  public:
   inline VoteResponseMessage(Packet& packet):
      Message(packet){
          pHeader->iType = MT_VoteResponse;
          setLength(sizeof(Header) + sizeof(Response));
      };

  struct Response {
    TermType       iTerm;
    bool           bVoteGranted;
    ServerIdType   iVoter;
  };

  Response* response()const{
    return  reinterpret_cast<Response*>(pHeader + 1);
  };

};
/*****************************************************************************/
class AppendEntriesRequest : public Message {
  public:
   inline AppendEntriesRequest(Packet& packet):
      Message(packet){
        pHeader->iType = MT_AppendEntriesRequest;
        setLength(sizeof(Header) + sizeof(Request));
      };

  struct Request {

      size_t         iLeaderId; //TODO
      LogIndexType   iPrevLogIndex;
      TermType       iPrevLogTerm;
      LogIndexType   iLeaderCommitIndex;
      TermType       iTerm;
      LogIndexType   iIndexId;
      uint16_t       iEntriesSize;
  };

  Request* request()const{
    return  reinterpret_cast<Request*>(pHeader + 1);
  };

  void *data(){
    return  reinterpret_cast<void*>(request() + 1);
  };

};
/*****************************************************************************/
class AppendEntriesResponse : public Message {
  public:
   inline AppendEntriesResponse(Packet& packet):
      Message(packet){
          pHeader->iType = MT_AppendEntriesResponse;
          setLength(sizeof(Header) + sizeof(Response));
      };

  struct Response {
    TermType       currentTerm;
    LogIndexType   iIndexId;
    uint16_t       iEntriesSize;
    bool           bSuccess;
  };

  Response* response()const{
    return    reinterpret_cast<Response*>(pHeader + 1);
  };

};

/*****************************************************************************/
}
}
}
}

#endif /* _IA20_Net_Engine_Raft_Message_H_ */
