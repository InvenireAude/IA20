/*
 * File: IA20-CommonLib/src/commonlib/exception/Exception.h
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
/* ChangeLog:
 *
 */


#ifndef _IA20_Exception_H_
#define _IA20_Exception_H_

#include <iostream>

namespace IA20{

/*************************************************************************/
/** The base exception class.
 */

 class Exception{
public:

	Exception();
	Exception(const std::string& strInfo);

	virtual ~Exception() throw();
	virtual const char*  getName();
	const std::string& getInfo();
	virtual std::string       toString();
	virtual void  printToStream(std::ostream& os);

	Exception& operator<< (const std::string& strInfo);
	Exception& operator<< (const char *sValue);
	Exception& operator<< (int iValue);
	Exception& operator<< (long iValue);
	Exception& operator<< (unsigned long iValue);
	Exception& operator<< (double fValue);
	Exception& operator<< (void* ptrValue);

protected:
	virtual void setInfo(const char *sInfo);
	virtual void setInfo(const std::string& strInfo);
	virtual void appendStackTrace();


private:
	std::string strInfo;
	std::string strStackTrace;
};

}

#endif /*EXCEPTION_H_*/
