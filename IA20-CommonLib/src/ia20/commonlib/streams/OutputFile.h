/*
 * File: IA20-CommonLib/src/commonlib/streams/OutputFile.h
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
#ifndef OUTPUTFILE_H_
#define OUTPUTFILE_H_

#include "File.h"
#include <fstream>
/*
 *
 */
namespace IA20 {

class OutputFile : public File {
public:

	OutputFile(const String& strName, bool bAppend = false);
	virtual ~OutputFile();

	virtual void open();
	virtual void close();

	std::ofstream& getStreamRef();

	operator std::ofstream&() { return getStreamRef(); }

	virtual bool isOpenedAndGood();

	static void SaveString(const String& strFileName, const String& strValue, bool bAppend = false);

protected:

	std::ofstream os;
  bool bAppend;

};

}

#endif /* OUTPUTFILE_H_ */
