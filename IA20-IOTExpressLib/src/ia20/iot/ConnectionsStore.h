/*
 * File: ConnectionsStore.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_ConnectionsStore_H_
#define _IA20_IOT_ConnectionsStore_H_

#include <ia20/commonlib/commonlib.h>

namespace IA20 {
namespace IOT {

/*************************************************************************/
/** The ConnectionsStore class.
 *
 */
class ConnectionsStore {
public:

	virtual ~ConnectionsStore() throw();


	ConnectionsStore();
protected:

};

/*************************************************************************/
}
}

#endif /* _IA20_IOT_ConnectionsStore_H_ */
