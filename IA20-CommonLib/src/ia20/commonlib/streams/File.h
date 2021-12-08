/*
 * File: IA20-CommonLib/src/commonlib/streams/File.h
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
#include "../logger/logger.h"
#include "../types.h"

#ifndef FILE_H_
#define FILE_H_

/*
 *
 */
namespace IA20 {

class File {
public:
	File(const String& strName);
	virtual ~File();

	virtual void open()=0;
	virtual void close()=0;

	virtual bool isOpenedAndGood()=0;

	virtual String& getName();

	static void Remove(const String& strName);
    static bool Exists(const String& strName);

protected:
	String strName;
};

}

#endif /* FILE_H_ */
