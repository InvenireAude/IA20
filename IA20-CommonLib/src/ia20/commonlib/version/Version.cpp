/*
 * File: IA20-CommonLib/src/commonlib/tools/Version.cpp
 *
 * Copyright (C) 2019, Albert Krzymowski
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
#include "Version.h"

#include <openssl/opensslv.h>
#include <ia20/commonlib/types.h>

namespace IA20 {

void Version::Print(std::ostream& os){

  const String CVersion ("0.0.1");

  os<<"IA20 CommonLib: "<<CVersion<<std::endl;
  os<<" Complied on:  "<<__DATE__<<" "<<__TIME__<<", environment: "<<__VERSION__<<std::endl;
  os<<"   OpenSSL  :  "<<OPENSSL_VERSION_TEXT<<std::endl;

}
/*************************************************************************/
}
