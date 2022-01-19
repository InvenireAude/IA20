/*
 * File: IA20-CommonLib-TestCases/src/testcases/TCSubscriptions.h
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
#ifndef _IA20_TC_TCSubscriptions_H_
#define _IA20_TC_TCSubscriptions_H_

#include <ia20/commonlib/commonlib.h>
#include <ia20/commonlib/testcase/TestUnit.h>

#include <ia20/iot/iot.h>
#include <ia20/iot/Subscription.h>
#include <ia20/iot/SubscriptionsStore.h>
#include <ia20/iot/Topic.h>
#include <ia20/iot/TopicsStore.h>

namespace IA20{
namespace TC{


class TCSubscriptions:
  public TestUnit<TCSubscriptions>{

public:

	TCSubscriptions(TestSuite* pTestSuite);
	virtual ~TCSubscriptions()  throw ();


	void caseSimple();
	void caseHash01();
	void caseHash02();
	void casePlus01();

	void caseRetained01();
	void caseRetained02();
  void caseRetained03();

  struct TestEnv {

    std::unique_ptr<IOT::SubscriptionsStore> ptrSubscriptionsStore;
    std::unique_ptr<IOT::TopicsStore>        ptrTopicsStore;

    void reset();
  };

protected:

   TestEnv env;

  void checkSubscriptions(
    const std::list< std::pair<int, String> >& lstSetup,
    const String& strPubTopic,
    const std::list< std::pair<int, String> >& lstResult);

  void checkRetained(
    const std::list< std::pair<int, String> >& lstSetup,
    const String& strPubTopic,
    const std::list< std::pair<int, String> >& lstResult);

};


}
}


#endif /*_IA20_TC_TCSubscriptions_H_*/
