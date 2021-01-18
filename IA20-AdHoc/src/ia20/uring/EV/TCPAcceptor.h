/*
 * File: TCPAcceptor.h
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *

 */


#ifndef _IA20_URing_EV_TCPAcceptor_H_
#define _IA20_URing_EV_TCPAcceptor_H_

#include <ia20/commonlib/commonlib.h>

#include <ia20/net/conn/tcp/Server.h>

#include "../EventHandler.h"

namespace IA20 {
namespace URing {
namespace EV {

/*************************************************************************/
/** The TCPAcceptor class.
 *
 */
class TCPAcceptor : public EventHandler {
public:

	virtual ~TCPAcceptor() throw();


	TCPAcceptor(Net::Conn::TCP::Server* pServer);

  void handle();

protected:


  Net::Conn::TCP::Server* pServer;
};

/*************************************************************************/
}
}
}

#endif /* _IA20_URing_EV_TCPAcceptor_H_ */
