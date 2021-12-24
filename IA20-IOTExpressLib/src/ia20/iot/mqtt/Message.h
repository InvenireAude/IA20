/*
 * File: Message.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_MQTT_Message_H_
#define _IA20_IOT_MQTT_Message_H_

#include <ia20/commonlib/commonlib.h>

namespace IA20 {
namespace IOT {
namespace MQTT {

/*************************************************************************/
/** The Message class.
 *
 */
class Message {
public:

	Message(){
     //TODO tmp for prototyping only
  };

  enum Type {
    MT_NONE     = 0,
    MT_CONNECT  = 1,
    MT_CONNACK  = 2,
    MT_PUBLISH  = 3,
    MT_PUBACK   = 4,
    MT_PUBREC   = 5,
    MT_PUBREL   = 6,
    MT_PUBCOMP  = 7,
    MT_SUBSCRIBE = 8,
    MT_SUBACK    = 9,
    MT_UNSUBSCRIBE = 10,
    MT_UNSUBACK    = 11,
    MT_PINGREQ     = 12,
    MT_PINGRESP    = 13,
    MT_DISCONNECT  = 14,
    MT_AUTH        = 15
  };

//protected:
  Type     iType;
  uint8_t  *pPayload;
  long      iMessageId;
  uint16_t iLen; //TODO tmp for prototyping only
};
/*************************************************************************/
}
}
}

#endif /* _IA20_IOT_MQTT_Message_H_ */
