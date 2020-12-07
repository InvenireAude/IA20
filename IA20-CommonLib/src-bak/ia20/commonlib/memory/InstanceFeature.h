/*
 * File: IA20-CommonLib/src/commonlib/misc/InstanceFeature.h
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
#ifndef _IA20_InstanceFeature_H_
#define _IA20_InstanceFeature_H_

/*
 *
 */

namespace IA20 {

template<class TT>
class InstanceFeature {
public:

	virtual ~InstanceFeature(){};

	static TT* GetInstance() {
		//It is it up to the user to initialize this safely.
		if(!TheInstance)
			TheInstance = new TT();
		return TheInstance;
	}

	/*
	 * For debug purposes only.
	 */

	static void Delete() {
    if(TheInstance)
         delete TheInstance;
    TheInstance = 0;
    }


protected:
	typedef TT* TheInstanceType;
	static TheInstanceType TheInstance;
};

template<class TT>
 typename InstanceFeature<TT>::TheInstanceType InstanceFeature<TT>::TheInstance(0);

}

#endif /* _IA20_InstanceFeature_H_ */
