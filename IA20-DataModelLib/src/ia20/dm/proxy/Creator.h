/*
 * File: Creator.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_DM_Proxy_Creator_H_
#define _IA20_DM_Proxy_Creator_H_

#include <ia20/commonlib/commonlib.h>

#include <ia20/dm/Type.h>
#include <ia20/dm/DataObject.h>

namespace IA20 {
namespace DM {
namespace Proxy {

/*************************************************************************/
/** The Creator class.
 *
 */
class Creator {
public:

	~Creator() throw();
	Creator();

  static Creator TheInstance;

protected:

};

/*************************************************************************/
}
}
}

#endif /* _IA20_DM_Proxy_Creator_H_ */
