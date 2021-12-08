/*
 * File: IA20-CommonLib/src/commonlib/logger/StackTrace.h
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

#ifndef _IA20_StackTrace_H_
#define _IA20_StackTrace_H_

#include <features.h>

namespace IA20{

/*************************************************************************/


#ifdef __GLIBC__
void PrintTrace(std::ostream& os);
#endif

}

#endif /*STACKTRACE_H_*/

