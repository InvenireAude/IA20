/*
 * File: Address.h
 *
 * Copyright (C) 2020, Albert Krzymowski
 *

 */

#ifndef _IA20_Net_Conn_Peer_H_
#define _IA20_Net_Conn_Peer_H_

#include <ia20/commonlib/commonlib.h>

#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

namespace IA20 {
namespace Net {
namespace Conn {
/*************************************************************************/
class Peer{
public:

	Peer();
	Peer(const Peer& other);
	Peer(int iPort);
	Peer(const String& strAddress, int iPort=-1);
	Peer(const String& strAddress, const String& strService);

	~Peer() throw();

	inline bool isDefault()const{ return strAddress.empty() && strService.empty(); };
	inline bool hasAddress()const{ return !strAddress.empty(); };
	inline bool hasService()const{ return !strService.empty(); };

	Peer& operator=(const Peer& other);

	const String& getAddress()const { return strAddress; }
	const String& getService()const { return strService; };

	operator String()const{
		StringStream ssTmp;
		ssTmp<<(*this);
		return ssTmp.str();
	}

	friend std::ostream& operator<<(std::ostream&, const Peer& peer);

protected:

	String strAddress;
	String strService;

};
/*************************************************************************/
std::ostream& operator<<(std::ostream&, const Peer& peer);
/*************************************************************************/
}
}
}

#endif /* _IA20_Net_Conn_Peer_H_ */
