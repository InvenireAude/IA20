/*
 * File: IA20-CommonLib/src/commonlib/tools/Template.cpp
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
#include "Template.h"
#include "../exception/ProcessorException.h"
#include "../exception/ItemNotFoundException.h"
#include "../exception/BadUsageException.h"

#include "../memory/memory.h"
#include "../logger/logger.h"


namespace IA20 {

/*************************************************************************/
Template::Arguments::Arguments(Arguments* pParentArguments):
		pParentArguments(pParentArguments)
	{};
/*************************************************************************/
Template::Arguments::~Arguments() throw(){}
/*************************************************************************/
void Template::Arguments::add(const std::string& strKey, const std::string& strValue){
	IA20_TRACER;

	hmValues[strKey]=strValue;
}
/*************************************************************************/
Template::Arguments* Template::Arguments::createNested(const std::string& strKey){

	Template::Arguments* pResult = createNestedImpl(strKey);

	if(hmNested.count(strKey) == 0)
		hmNested[strKey]=IA20_DFT_FACTORY<NestedEntry>::Create();
	hmNested[strKey]->push(pResult);

	return pResult;
}
/*************************************************************************/
const std::string& Template::Arguments::get(const std::string& strKey){
	IA20_TRACER;

	ValuesMap::const_iterator it=hmValues.find(strKey);

	if(it == hmValues.end()) {

		std::string strValue;

		if(getImpl(strKey,strValue)){
			hmValues[strKey] = strValue;
			return hmValues[strKey];
		}

		if(pParentArguments)
			return pParentArguments->get(strKey);

		IA20_THROW(ItemNotFoundException(strKey));
	}

	return it->second;
}
/*************************************************************************/
size_t Template::Arguments::getNestedCount(const std::string& strKey){

	if(hmNested.count(strKey) == 0) {

		if(pParentArguments)
			return pParentArguments->getNestedCount(strKey);

		IA20_THROW(ItemNotFoundException(strKey));
	}

	return hmNested.at(strKey)->tabArguments.size();
}
/*************************************************************************/
Template::IArguments* Template::Arguments::getNested(const std::string& strKey, size_t iIdx){
	IA20_TRACER;

	if(hmNested.count(strKey) == 0) {

		if(pParentArguments)
			return pParentArguments->getNested(strKey,iIdx);

		IA20_THROW(ItemNotFoundException(strKey));
	}

	if(hmNested.at(strKey)->tabArguments.size() <= iIdx)
		IA20_THROW(ItemNotFoundException(strKey + " index out of bound"));

	return hmNested.at(strKey)->tabArguments[iIdx];
}
/*************************************************************************/
Template::Arguments* Template::Arguments::createNestedImpl(const std::string& strKey){
	IA20_TRACER
	return IA20_DFT_FACTORY<Arguments>::Create(this);
}
/*************************************************************************/
bool Template::Arguments::getImpl(const std::string& strKey, std::string& strValue){
	IA20_TRACER;
	return false;
}
/*************************************************************************/
Template::Template(const std::string& strTemplateText){

	IA20_TRACER;
	this->strTemplateText=strTemplateText;
}
/*************************************************************************/
Template::~Template() throw(){
	IA20_TRACER;
}
/*************************************************************************/
void Template::evaluate(IArguments& args, std::ostream& os)const{

	State iState = ST_Outside;
	std::string strKey;
	std::string strRepeatKey;
	std::stringstream ssRepeat;

	std::ostream* posActive(&os);

	for (int iIdx = 0; iIdx < strTemplateText.length(); iIdx++) {

		char c = strTemplateText[iIdx];

		switch (c) {

		case '$':
			switch (iState) {
			case ST_Outside:
				iState = ST_Special;
				break;
			case ST_Special:
				iState = ST_Outside;
				(*posActive) << "$$";
				break;
			case ST_Template:
				IA20_THROW(ProcessorException("$ within Template."));
				break;
			default:
				IA20_THROW(InternalException("Template parse error 1"));
			}
			break;

		case '{':
			switch (iState) {
			case ST_Outside:
				(*posActive) << c;
				break;
			case ST_Special:
				iState = ST_Template;
				strKey = "";
				break;
			case ST_Template:
				IA20_THROW(ProcessorException("{ within Template."));
				break;
			default:
				IA20_THROW(InternalException("Template parse error 2"));
			}
			break;

		case '}':
			switch (iState) {
			case ST_Outside:
				(*posActive) << c;
				break;
			case ST_Special:
				iState = ST_Outside;
				(*posActive) << c;
				break;

			case ST_Template:

				if(strKey.length() == 0)
					IA20_THROW(ItemNotFoundException("Empty template key ?"));

				if(strKey[0] == '*'){
					strKey=strKey.substr(1);
					if(strRepeatKey.empty()){
						strRepeatKey=strKey;
						posActive = &ssRepeat;

					}else{

						if(strRepeatKey.compare(strKey) == 0){
							Template t(ssRepeat.str());
							for(int iIdx = 0; iIdx < args.getNestedCount(strKey); iIdx++)
								t.evaluate(*args.getNested(strKey,iIdx),os);
							strRepeatKey.clear();
							posActive=&os;

						}else{
							(*posActive)<<"${*"<<strKey<<"}";
						}

					}
				}else{

					if(posActive == &os){

						bool bIgnoreNotSet = strKey[0] == '?';

						if(bIgnoreNotSet)
							strKey=strKey.substr(1);

						try{
							(*posActive) << args.get(strKey);
						}catch(ItemNotFoundException& e){
							if(!bIgnoreNotSet)
								throw;
						}

					}else
						(*posActive) << "${"<<strKey <<"}";
				}
				iState = ST_Outside;
				break;
			default:
				IA20_THROW(InternalException("Template::buildKeysList::iState"))
				;
			}
			break;

		default:
			switch (iState) {
			case ST_Outside:
				(*posActive) << c;
				break;
			case ST_Special:
				break;
			case ST_Template:
				strKey += c;
				break;
			default:
				IA20_THROW(InternalException("Template::buildKeysList::iState"))
				;
			}
		}/* SWITCH (c) */

	}/* FOR */

	if(iState == ST_Special)
		os<<'$';

	if(iState == ST_Template)
		os<<"${"<<strKey;

	if(posActive != &os)
		IA20_THROW(BadUsageException("Parse error in a template interation: "+strRepeatKey));

	os.flush();
}

/*************************************************************************/
}

