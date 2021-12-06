/*
 * File: IA20-CommonLib/src/commonlib/tools/ProgramParameters.h
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
#ifndef _IA20_ProgramParameters_H_
#define _IA20_ProgramParameters_H_

#include "../types.h"

#include <map>
#include <vector>

namespace IA20 {

/*************************************************************************/
/** The class. */
 class ProgramParameters {
public:

	virtual ~ProgramParameters() throw();

	typedef StringList ParametersList;

	const ParametersList& getParameters() const;

	const String& getExeName() const;
protected:
	ProgramParameters()throw();
	void init(int argc, char* argv[], const String& strArgSpec);

	typedef std::map<char, String> ArgValuesMap;
	typedef std::map<char, bool>  IndicatorsMap;

  const String& getStringValue(char cOption, const String& strDefault = "__none__")const;
  Long  getLongValue(char cOption, long iDefault = -999999 )const;
  int  getIntValue(char cOption, int iDefault = -999999 )const;

	ArgValuesMap   hmValues;
	IndicatorsMap  hmIndicators;
	ParametersList lstParameters;
	String         strExeName;
};

/*************************************************************************/
}

#endif /* _IA20_ProgramParameters_H_ */
