/*
 * File: IA20-CommonLib/src/commonlib/logger/StackTrace.cpp
 *
 * Copyright (C) 2015, Albert Krzymowski
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

#include "StackTrace.h"

#ifdef __GLIBC__
#include <execinfo.h>
#endif

#include <iostream>

namespace IA20 {

void PrintTrace(std::ostream& os){

  void *array[25];
  size_t size;
  char **strings;
  size_t i;

  size = backtrace (array, 25);
  strings = backtrace_symbols (array, size);

  os<<"Obtained:<<"<<size<<"stack frames, (DO NOT PANIC !!!, this is IA20 stacktrace printed with glibc backtrace features :] )"<<std::endl;

  for (i = 0; i < size; i++)
     os<<strings[i]<<std::endl;

  free (strings);
}


}/* namespace IA20 */

