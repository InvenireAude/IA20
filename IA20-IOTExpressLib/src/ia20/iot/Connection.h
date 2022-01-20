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
#include <ia20/iot/memory/FixedObjectsTable.h>

#include <ia20/iot/logger/LogLevel.h>

#include <string.h>

#include <wchar.h>

namespace IA20 {
namespace IOT {

class Activity;

/*************************************************************************/
/** The Connection class.
 *
 */
class Connection :
    public Tools::IdentifiedByHandle {
public:

    typedef uint16_t PacketIdentifierType;

    ~Connection();
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

    PacketIdentifierType addOutputActivity(Activity* pActivity){
        Activity** pEntry = tabOutputActivies.allocate(pActivity);
        IA20_LOG(IOT::LogLevel::INSTANCE.isDetailedInfo(), "Packet Identifier (add):    ["<<(int)getHandle()<<"]"<<(int)tabOutputActivies.pointerToIdx(pEntry) + 1);
        return tabOutputActivies.pointerToIdx(pEntry) + 1;
    }

    Activity* getOutputActivity(PacketIdentifierType iPacketId){
        return *tabOutputActivies.idxToPonter(iPacketId);
    }

    void removeOutputActivity(PacketIdentifierType iPacketId){
        tabOutputActivies.free(iPacketId - 1);
        IA20_LOG(IOT::LogLevel::INSTANCE.isDetailedInfo(), "Packet Identifier (remove): ["<<(int)getHandle()<<"]"<<(int)iPacketId);        
    }

    inline uint8_t getMaxQoS()const{
        return 1; // TODO client policy
    }


protected:

    unsigned int iListener;
    unsigned int iListenerConnectionId;

    wchar_t utfClientId[2 * CMaxClientIdLen + 1];
    MQTT::Message::VersionType iMQTTVersion;

    static const int CMaxActivitesPerConnection = 10;

    typedef Memory::FixedObjectsTable<Activity*> ActivityTable;
    
    ActivityTable tabInputActivies;
    ActivityTable tabOutputActivies;
};
/*************************************************************************/
}
}

#endif /* _IA20_IOT_Connection_H_ */
