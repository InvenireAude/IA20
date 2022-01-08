/*
 * File: IA20-CommonLib-TestCases/src/testcases/TCMQTTServer.h
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
#ifndef _IA20_TC_TCMQTTServer_H_
#define _IA20_TC_TCMQTTServer_H_

#include <ia20/commonlib/commonlib.h>
#include <ia20/commonlib/testcase/TestUnit.h>

#include <ia20/iot/iot.h>
#include <ia20/iot/mocker/mocker.h>
#include <ia20/iot/tools/sys/TasksRing.h>
#include <ia20/iot/tools/spin/TasksRing.h>

namespace IA20{
namespace TC{


class TCMQTTServer:
  public TestUnit<TCMQTTServer>{

public:

	TCMQTTServer(TestSuite* pTestSuite);
	virtual ~TCMQTTServer()  throw ();

	void caseBasic();

protected:

  struct TestEnv {

    std::unique_ptr<IOT::Mocker::ActivityStore> ptrActivityStore;
    std::unique_ptr<IOT::Mocker::ActionsStore>  ptrActionsStore;
    std::unique_ptr<IOT::Mocker::Listener>      ptrListener;


    std::unique_ptr<IOT::Engine> ptrEngine;

    void reset();
  };

  struct TestEnv env;

};


}
}


#endif /*_IA20_TC_TCMQTTServer_H_*/
