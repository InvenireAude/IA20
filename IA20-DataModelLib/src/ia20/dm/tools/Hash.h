/*
 * File: Hash.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_DM_Tools_Hash_H_
#define _IA20_DM_Tools_Hash_H_

#include <ia20/commonlib/commonlib.h>

namespace IA20 {
namespace DM {
namespace Tools {

/*************************************************************************/
/** The Hash class.
 *
 */
class Hash {
public:

  struct StringHash {
   size_t operator() (const String &strValue) const {
     return std::hash<std::string>()(strValue.c_str());
   }
  };
  typedef std::pair<const String&, const String&> KeyPairType;

  struct StringRefPairHash {
   size_t operator() (const KeyPairType &aPair) const {
     std::string temp(aPair.first + "#" + aPair.second);
     return std::hash<std::string>()(temp);
   }
  };
};

/*************************************************************************/
}
}
}

#endif /* _IA20_DM_Tools_Hash_H_ */
