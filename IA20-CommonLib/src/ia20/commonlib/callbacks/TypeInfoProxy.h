/*
 * File: IA20-CommonLib/src/commonlib/callbacks/TypeInfoProxy.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/* IA20_COPYRIGHT */

/* ChangeLog:
 *
 */

#ifndef TYPEINFOPROXY_H_
#define TYPEINFOPROXY_H_

#include <typeinfo>

/*
 *
 */
namespace IA20 {

/*************************************************************************/
/** The encapsulates operators on std::type_info.
 *
 */
class TypeInfoProxy {
  public:
    TypeInfoProxy(const std::type_info * t) : ti(t) {}
    const std::type_info * ti;
};

inline bool operator<(const TypeInfoProxy & lhs, const TypeInfoProxy & rhs) {
  return lhs.ti->before(*rhs.ti) != 0;
}

inline bool operator>(const TypeInfoProxy & lhs, const TypeInfoProxy & rhs) {
  return rhs.ti->before(*lhs.ti) != 0;
}

inline bool operator<=(const TypeInfoProxy & lhs, const TypeInfoProxy & rhs) {
  return !(rhs > lhs);
}

inline bool operator>=(const TypeInfoProxy & lhs, const TypeInfoProxy & rhs) {
  return !(rhs < lhs);
}

inline bool operator==(const TypeInfoProxy & lhs, const TypeInfoProxy & rhs) {
  return ((*lhs.ti) == (*rhs.ti)) != 0;
}

inline bool operator!=(const TypeInfoProxy & lhs, const TypeInfoProxy & rhs) {
  return ((*lhs.ti) != (*rhs.ti)) != 0;
}


}

#endif /* TYPEINFOPROXY_H_ */
