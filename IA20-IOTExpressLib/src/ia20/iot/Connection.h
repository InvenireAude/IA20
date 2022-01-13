/*
 * File: Connection.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_Connection_H_
#define _IA20_IOT_Connection_H_

#include <ia20/commonlib/commonlib.h>
#include <ia20/iot/mqtt/Message.h>
#include <ia20/iot/tools/IdentifiedByHandle.h>
#include <ia20/iot/tools/StringRef.h>

#include <string.h>

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
               wchar_t*     utfClientId);

    unsigned int getListenerIdx()const{
        return iListener;
    }

    static const size_t CMaxClientIdLen = 23;

    inline MQTT::Message::VersionType getMQTTVersion()const{
        return iMQTTVersion;
    }

    inline void setVersion(MQTT::Message::VersionType iMQTTVersion){
        this->iMQTTVersion = iMQTTVersion;
    }

    inline Tools::StringRef getClientId()const{
        return  Tools::StringRef((const uint8_t *)utfClientId, strnlen((const char*)utfClientId, 2*CMaxClientIdLen) );
    }

    inline uint16_t getNextId(){
        return iNextId++;
    }
    
protected:

    unsigned int iListener;
    unsigned int iListenerConnectionId;

    wchar_t utfClientId[2 * CMaxClientIdLen + 1];
    MQTT::Message::VersionType iMQTTVersion;

    uint16_t iNextId;
};
/*************************************************************************/
}
}

#endif /* _IA20_IOT_Connection_H_ */
