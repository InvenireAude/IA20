/*
 * File: Hash.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_Tools_Hash_H_
#define _IA20_IOT_Tools_Hash_H_

#include <ia20/commonlib/commonlib.h>

namespace IA20 {
namespace IOT {
namespace Tools {

/*************************************************************************/
/** The Hash class.
 *
 */
class MQTT {
public:

  typedef uint32_t VLIntType;

  static inline uint8_t *encodeVL(uint8_t* pData, VLIntType iValue){

    do {
      
        uint8_t encodedByte = iValue % 128; //TODO >>      
        iValue = iValue / 128; // TODO
        
        if (iValue > 0)
          encodedByte = encodedByte | 0x80;
        
        *pData++ = encodedByte;

      }while (iValue > 0);

      return pData;
   }

   static inline VLIntType decodeVL(uint8_t* pData){
      VLIntType iValue;
      decodeVL(pData, iValue);
      return iValue;
   }

   static inline uint8_t* decodeVL(uint8_t* pData, VLIntType& iValue){

     uint32_t m = 1;
     iValue = 0;

     uint8_t iByte;

     do {

      iByte = *pData++;
      iValue += (iByte & 0x7f) * m;
      
      if (m > 128*128*128)
        IA20_THROW(InternalException("Add exception for decodeVL"));
     
      m *= 128;

     }while ((iByte & 0x80) != 0);

    return pData;
   }

  static inline bool hasFixedHeader(uint8_t* pData, uint32_t iLength){

     if(iLength > 5)
      return true;

     if(iLength < 2)
      return false;
      
     iLength--;
     pData++;

     uint8_t iByte;

     do {

       if(iLength-- == 0)
        return false;
        
       iByte = *pData++;

     }while ((iByte & 0x80) != 0);

    return true;
   }


};
/*************************************************************************/
}
}
}

#endif /* _IA20_IOT_Tools_Hash_H_ */
