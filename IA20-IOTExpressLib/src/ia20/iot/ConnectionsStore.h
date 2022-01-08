/*
 * File: ConnectionsStore.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_ConnectionsStore_H_
#define _IA20_IOT_ConnectionsStore_H_

#include <ia20/commonlib/commonlib.h>
#include <ia20/iot/memory/FixedObjectsPool.h>

#include "Connection.h"

namespace IA20 {
namespace IOT {

/*************************************************************************/
/** The ConnectionsStore class.
 *
 */
class ConnectionsStore :
	 public Tools::HandleSetter{
public:

	virtual ~ConnectionsStore() throw();
	ConnectionsStore();

	Connection *create(
				unsigned int iListener, 
                wchar_t*   utfClientId);

	Connection *get(Connection::HandleType);

protected:

	typedef Memory::FixedObjectsPool<Connection, 1000> ConnectionsPool;

	long iCounter;
};
/*************************************************************************/
}
}

#endif /* _IA20_IOT_ConnectionsStore_H_ */
