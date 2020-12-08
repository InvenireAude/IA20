/*
 * File: cif.h
 *
 * Copyright (C) 2020, Albert Krzymowski
 *

 */


#ifndef _IA20_FB_Helpers_H_
#define _IA20_FB_Helpers_H_

#include <ia20/commonlib/commonlib.h>
#include <flatbuffers/minireflect.h>

namespace IA20 {
namespace FB {

struct Helpers{

public:

//This is only for debugging or testing, etc !!! .
static inline String ToString(const uint8_t *buffer, const flatbuffers::TypeTable *type_table){
  flatbuffers::ToStringVisitor visitor("\n", true, " ", true);
  IterateFlatBuffer(buffer, type_table, &visitor);
  return visitor.s.c_str();
}

};

}
}

#endif /* _IA20_FB_Helpers_H_ */
