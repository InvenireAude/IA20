/*
 * File: IA20-CommonLib/src/commonlib/logger/StackTrace.cpp
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
#include "StackTrace.h"

#ifdef __GLIBC__
#include <execinfo.h>
#endif

namespace IA20{

StackTrace::StackTrace() throw(){
}

StackTrace::~StackTrace() throw(){
}

StackTrace::Entry::Entry(const char *sNewFun,
						 const char *sNewFile,
						 int        iNewLine):
  sFun(sNewFun),
  sFile(sNewFile),
  iLine(iNewLine)
  {}

StackTrace::Entry::Entry(const Entry& e):
  sFun(e.sFun),
  sFile(e.sFile),
  iLine(e.iLine)
  {}

void StackTrace::addEntry(const char *sFun,
						  const char *sFile,
				          int        iLine){
	Entry e(sFun,sFile,iLine);
	lstStackTrace.push_back(e);
}

void StackTrace::removeEntry(){
	if(lstStackTrace.size() > 0)
		lstStackTrace.pop_back();
}


bool StackTrace::empty() const{
	return lstStackTrace.size() == 0;
}

size_t StackTrace::getSize() const{
	return lstStackTrace.size();
}

const StackTrace::Entry &StackTrace::top() const{
	if(empty())
		throw std::exception();//TODO (LL) what to throw safely. Maybe return a dummy object.

	return lstStackTrace.back();
}
void StackTrace::printStack(std::ostream& os){
	int iIdx;

	os<<"\n\t Stack trace["<<((unsigned int)pthread_self())<<"]:\n\n";
	for(iIdx=lstStackTrace.size()-1; iIdx >= 0; iIdx--){
		   Entry& en=lstStackTrace[iIdx];
		   os<<en.sFile;
		   os<<"(";
		   os<<en.iLine;
		   os<<")";
		   os<<": ";
		   os<<en.sFun;
		   os<<"\n";
	};
	os<<"\n\t <<<<<>>>>>\n";
 }

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

