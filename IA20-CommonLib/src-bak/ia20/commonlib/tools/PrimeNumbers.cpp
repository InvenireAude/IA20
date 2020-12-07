/*
 * File: IA20-CommonLib/src/commonlib/tools/PrimeNumbers.cpp
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
#include "PrimeNumbers.h"

namespace IA20 {
static const int CNumPrimes = 64;
static unsigned int _primes[CNumPrimes] = {
2,
3,
5,
7,
11,
13,
17,
19,
23,
29,
31,
37,
53,
67,
83,
107,
139,
181,
239,
307,
401,
521,
677,
881,
1151,
1487,
1931,
2521,
3271,
4253,
5519,
7177,
9337,
12143,
15767,
20507,
26647,
34649,
45053,
58537,
76099,
98929,
128621,
167191,
217351,
282559,
367321,
477511,
620771,
806999,
1049093,
1363837,
1772971,
2304851,
2996317,
3895231,
5063759,
6582889,
8557763,
11125091,
14462639,
18801389,
24441799,
31774361};
/*************************************************************************/
unsigned int PrimeNumbers::GetNextGtEq(unsigned int iValue){

	IA20_TRACER;

	int a=0;
	int b=CNumPrimes-1;

	if(iValue > _primes[b]){
		IA20_LOG(LogLevel::INSTANCE.isError(),"Requested: "<<iValue<<", max is: "<<_primes[b]);
		IA20_THROW(InternalException("I do not have such a big number listed !"));
	}

	while(a < b){

		int i = (a + b) / 2;

		if(_primes[i] < iValue){
			a = i + 1;
		}else if(_primes[i] > iValue){
			b = i - 1;
		}else
			return iValue;

	}

	return _primes[b+1];

}
/*************************************************************************/
}
