/*
 * File: DefaultConnectionFactory.h
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *

 */


#ifndef _IA20_Net_Conn_TCP_DefaultConnectionFactory_H_
#define _IA20_Net_Conn_TCP_DefaultConnectionFactory_H_

#include <ia20/commonlib/commonlib.h>

#include "ConnectionFactory.h"

namespace IA20 {
namespace Net {
namespace Conn {
namespace TCP {

/*************************************************************************/
/** The DefaultConnectionFactory class.
 *
 */
class DefaultConnectionFactory
  : public ConnectionFactory,
    public InstanceFeature<DefaultConnectionFactory>{
public:

	virtual ~DefaultConnectionFactory() throw();

  virtual FileHandle* createFileHandle(int fd, const Peer& peer);

	DefaultConnectionFactory();
protected:

};

/*************************************************************************/
}
}
}
}

#endif /* _IA20_Net_Conn_TCP_DefaultConnectionFactory_H_ */
