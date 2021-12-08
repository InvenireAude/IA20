/*
 * File: IA20-CommonLib/src/commonlib/tools/ProgramParameters.cpp
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
#include "ProgramParameters.h"

#include "../logger/logger.h"

#include "../exception/BadUsageException.h"
#include "../exception/ConfigException.h"
#include "../tools/TypeTools.h"

#ifndef _AIX
#include <getopt.h>
#endif

namespace IA20 {

/*************************************************************************/
ProgramParameters::ProgramParameters() throw(){
	IA20_TRACER;
}

/*************************************************************************/
ProgramParameters::~ProgramParameters() throw(){
	IA20_TRACER;
}
/*************************************************************************/
const ProgramParameters::ParametersList& ProgramParameters::getParameters() const{
	IA20_TRACER;
	return lstParameters;
}
/*************************************************************************/
const String& ProgramParameters::getExeName() const{
	return strExeName;
}
/*************************************************************************/
void ProgramParameters::init(int argc, char* argv[], const String& strArgSpec){
	IA20_TRACER;

	int c;

	while( (c = getopt(argc,argv, strArgSpec.c_str() )) != -1 )
	 switch(c){

		case ':':
			IA20_THROW(BadUsageException("Missing option value."));
			break;

		case '?':
			IA20_THROW(BadUsageException("Unexpected program argument."));
			break;

		default:

			hmIndicators[c]=true;

			if(optarg != NULL){
				hmValues[c]=optarg;
				IA20_LOG(IA20::LogLevel::INSTANCE.isInfo(),"["<<c<<"]="<<optarg);
			}else{
				IA20_LOG(IA20::LogLevel::INSTANCE.isInfo(),"["<<c<<"]");
			}
	}

	for(int iIdx = optind; iIdx<argc; iIdx++)
		lstParameters.push_back(String(argv[iIdx]));

	strExeName=argv[0];
	String::size_type iPos = strExeName.find_last_of('/');
	if(iPos != String::npos)
		strExeName = strExeName.substr(iPos+1);

}
/*************************************************************************/
const String& ProgramParameters::getStringValue(char cOption, const String& strDefault)const{
  IA20_TRACER;

  ArgValuesMap::const_iterator it = hmValues.find(cOption);
	if(it == hmValues.end()){

      if(!strDefault.compare("__none__")){
  		  IA20_THROW(ConfigException(String("Missing -") + cOption + " option."));
      }else{
        return strDefault;
      }

  }
	return it->second;
}
/*************************************************************************/
int ProgramParameters::getIntValue(char cOption, int iDefault)const{

  ArgValuesMap::const_iterator it = hmValues.find(cOption);

  if(it == hmValues.end()){

      if(iDefault == -999999){
  		  IA20_THROW(ConfigException(String("Missing -") + cOption + " option."));
      }else{
        return iDefault;
      }

  }
	return TypeTools::StringToInt(it->second);
}
/*************************************************************************/
long ProgramParameters::getLongValue(char cOption, long iDefault)const{

  ArgValuesMap::const_iterator it = hmValues.find(cOption);

  if(it == hmValues.end()){

      if(iDefault == -999999){
  		  IA20_THROW(ConfigException(String("Missing -") + cOption + " option."));
      }else{
        return iDefault;
      }

  }
	return TypeTools::StringToInt(it->second);
}
/*************************************************************************/
}
