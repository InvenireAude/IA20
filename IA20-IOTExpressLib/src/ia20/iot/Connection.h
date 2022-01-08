/*
 * File: Connection.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_Connection_H_
#define _IA20_IOT_Connection_H_

#include <ia20/commonlib/commonlib.h>
#include <ia20/iot/tools/IdentifiedByHandle.h>

#include <wchar.h>

namespace IA20 {
namespace IOT {

/*************************************************************************/
/** The Connection class.
 *
 */
class Connection :
    public Tools::IdentifiedByHandle {
public:


	Connection(unsigned int iListener, 
               HandleType   aHandle,
               wchar_t* utfClientId);

    static const size_t CMaxClientIdLen = 23;

protected:

    unsigned int iListener;
    unsigned int iListenerConnectionId;

    wchar_t utfClientId[2 * CMaxClientIdLen + 1];
};
/*************************************************************************/
}
}

#endif /* _IA20_IOT_Connection_H_ */
