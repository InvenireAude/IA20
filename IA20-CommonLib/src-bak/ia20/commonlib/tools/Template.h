/*
 * File: IA20-CommonLib/src/commonlib/tools/Template.h
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
#ifndef _IA20_Tmplate_H_
#define _IA20_Template_H_

#include <ia20/commonlib/types.h>
#include <ostream>

#include <map>
#include <vector>

#include "../containers/HashMapStringToPointer.h"
#include "../containers/HashMapOfPointers.h"

namespace IA20 {
/*************************************************************************/
class Template {
public:

	class IArguments {

		public:
			virtual ~IArguments(){};

			virtual const std::string& get(const std::string& strKey) = 0;
			virtual size_t getNestedCount(const std::string& strKey) = 0;
			virtual IArguments* getNested(const std::string& strKey, size_t iIdx) = 0;
	};

	class Arguments : public IArguments{

		public:
			virtual ~Arguments() throw();
			Arguments(Arguments* pParentArguments = NULL);

			void add(const std::string& strKey, const std::string& Value);
		    Arguments* createNested(const std::string& strKey);

			// non const as we allow caching
			virtual const std::string& get(const std::string& strKey);

			virtual size_t getNestedCount(const std::string& strKey);
			virtual IArguments* getNested(const std::string& strKey, size_t iIdx);

		protected:

			struct NestedEntry {
				virtual ~NestedEntry(){};

				typedef std::vector<Arguments*> ArgumentsTab;
				ArgumentsTab tabArguments;

				IA20_DFT_FACTORY<Arguments>::PtrHoldersCollection phcArguments;

				void push(Arguments* pArguments){
					tabArguments.push_back(pArguments);
					phcArguments.addPointer(pArguments);
				}
			};

			typedef HashMapWithStringKey<std::string> ValuesMap;
			typedef HashMapStringToPointer<NestedEntry> NestedMap;

			ValuesMap hmValues;
			NestedMap hmNested;

			Arguments* pParentArguments;

			//Override those in subclasses ...
			virtual Arguments* createNestedImpl(const std::string& strKey);
			virtual bool getImpl(const std::string& strKey, std::string& strValue);

	};

	virtual ~Template() throw();

	void evaluate(IArguments& args, std::ostream& os)const;

protected:
	enum State{
		ST_Outside = 0,
		ST_Special = 1,
		ST_Template = 2
	};

	Template(const std::string& strTemplateText);

	std::string strTemplateText;

	friend class ::IA20::Factory<Template>;
};

/*************************************************************************/
}
#endif /* _IA20_SDOWrap_Cpp_TEMPLATE_H_ */
