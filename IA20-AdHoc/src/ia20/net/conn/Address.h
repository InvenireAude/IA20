/*
 * File: Address.h
 *
 * Copyright (C) 2020, Albert Krzymowski
 *

 */


#ifndef _IA20_Net_Conn_Address_H_
#define _IA20_Net_Conn_Address_H_

#include <ia20/commonlib/commonlib.h>
#include <sys/types.h>
#include <sys/socket.h>

using namespace IA20;

namespace IA20 {
namespace Net {
namespace Conn {


/*************************************************************************/
/** The Address class.
 *
 */
class Address {
public:

  typedef uint64_t HashType;

  struct sockaddr_storage address;
  socklen_t               iAddressLen;

  HashType computeHash()const;

  bool operator==(const Address& other)const;

  Address(){
    reset();
  }

  void reset(){
    iAddressLen = sizeof(address);
  }

};
/*************************************************************************/
std::ostream& operator<<(std::ostream& os, const Address& a);
/*************************************************************************/
}
}
}

#endif /* _IA20_Net_Conn_Address_H_ */
