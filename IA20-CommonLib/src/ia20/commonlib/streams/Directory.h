/*
 * File: IA20-CommonLib/src/commonlib/streams/Directory.h
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
#ifndef DIRECTORY_H_
#define DIRECTORY_H_

#include "../types.h"

/*
 *
 */
namespace IA20 {

class Directory {
public:
	Directory();
	virtual ~Directory();


	static void CreateDirectory(const String& strDirectoryName);
	static void CreateDirectoryIfNecessary(const String& strDirectoryName);

};

}

#endif /* DIRECTORY_H_ */
