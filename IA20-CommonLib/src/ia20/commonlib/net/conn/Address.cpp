/*
 * File: Address.cpp
 *
 * Copyright (C) 2020, Albert Krzymowski
 *
 */

#include <ia20/commonlib/commonlib.h>

#include "Address.h"

#include <netinet/in.h>
#include <string.h>

namespace IA20 {
namespace Net {
namespace Conn {
/*************************************************************************/
 bool Address::operator==(const Address& other)const{

    if(address.ss_family == other.address.ss_family)
      return false;

    size_t iSize = 0;

  switch (address.ss_family){
    case AF_INET:
        iSize = sizeof(struct sockaddr_in);
      break;
    case AF_INET6:
        iSize = sizeof(struct sockaddr_in); // TODO IP V6??
        break;
    default:
      IA20_LOG(IA20::LogLevel::INSTANCE.isDetailedInfo(), "Unknown: "<<address.ss_family);
    }

  return memcmp(&address, &other.address, iSize) == 0;
}
/*************************************************************************/
Address::HashType Address::computeHash()const{
  IA20_TRACER;

  HashType iValue = 0;
  const uint8_t *pData  = reinterpret_cast<const uint8_t*>(&address);

  // TODO faster and better !
  for(int i = 0; i < iAddressLen; i++){
    uint64_t tmp = *pData++;
    tmp <<= 8 *(i & 0x3);
    iValue ^= tmp;
    iValue *= 13;
  };

  IA20_LOG(IA20::LogLevel::INSTANCE.isInfo(), "srcHash: "<<(void*)iValue); //nice hex ;)

  return iValue;
}
/*************************************************************************/
std::ostream& operator<<(std::ostream& os, const Address& a){

  switch (a.address.ss_family){
    case AF_INET:{
            const struct sockaddr_in* p = reinterpret_cast<const struct sockaddr_in*>(&a.address);
            os<<(p->sin_addr.s_addr  & 0x000000ff)<<".";
            os<<((p->sin_addr.s_addr & 0x0000ff00) >> 8)<<".";
            os<<((p->sin_addr.s_addr & 0x00ff0000) >> 16)<<".";
            os<<((p->sin_addr.s_addr & 0xff000000) >> 24)<<":"<<ntohs(p->sin_port);
    }
      break;
    case AF_INET6:
        os<<"!! TODO IPv6 !!";
        break;
    default:
      os<<"unknown ip ?";
    }
  return os;
}
/*************************************************************************/
}
}
}
