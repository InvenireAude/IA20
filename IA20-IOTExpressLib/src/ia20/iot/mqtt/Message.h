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

	Message(){ //TODO tmp for prototyping only
  };

  enum Type {
    MT_CONNECT  = 1
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
