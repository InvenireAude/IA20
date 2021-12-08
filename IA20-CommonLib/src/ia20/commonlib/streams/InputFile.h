/*
 * File: IA20-CommonLib/src/commonlib/streams/InputFile.h
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
#ifndef INPUTFILE_H_
#define INPUTFILE_H_

#include "./File.h"

#include <fstream>

/*
 *
 */
namespace IA20 {

class InputFile :public File {
public:
	InputFile(const String& strName);
	virtual ~InputFile();

	virtual void open();
	virtual void close();

	std::istream& getStreamRef();
	operator std::istream&() { return getStreamRef(); }

	virtual bool isOpenedAndGood();

	static void LoadString(const String& strFileName, String& strValue);
	static void LoadStdin(String& strValue);

protected:
	std::ifstream is;
};

}

#endif /* INPUTFILE_H_ */
