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

	Message(const String& strHex){ //TODO tmp for prototyping only
    MiscTools::HexToBinary(strHex, tabData, 4000);
    iLen = strHex.length()/2;
  };

  enum Type {
    MT_CONNECT  = 1
  };

  friend std::ostream& operator<<(std::ostream&os, const Message& m){
     os<<MiscTools::BinarytoHex(m.tabData, m.iLen);
     return os;
   }

protected:
  uint8_t tabData[4000];//TODO tmp for prototyping only
  uint16_t iLen;//TODO tmp for prototyping only
};
/*************************************************************************/
}
}
}

#endif /* _IA20_IOT_MQTT_Message_H_ */
