/*
 * File: IA20-CommonLib/src/commonlib/time/TimeFrame.cpp
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
#include "../logger/logger.h"

#include "TimeFrame.h"

#include "../memory/memory.h"

namespace IA20 {

/*************************************************************************/
TimeFrame::TimeFrame() throw(){
	IA20_TRACER;
}
/*************************************************************************/
TimeFrame::TimeFrame(const TimeFrame& other) throw(){
	IA20_TRACER;
	tsStart=other.tsStart;
	tsEnd=other.tsEnd;
}
/*************************************************************************/
TimeFrame& TimeFrame::operator=(const TimeFrame& other){
	IA20_TRACER;
	tsStart=other.tsStart;
	tsEnd=other.tsEnd;
  return *this;
}
/*************************************************************************/
TimeFrame::~TimeFrame() throw(){
	IA20_TRACER;
}
/*************************************************************************/
long TimeFrame::getDifferenceInSeconds(const Timestamp& ts1, const Timestamp& ts2)const{
	IA20_TRACER
	time_t t1;
	time_t t2;
	ts1.toPosixTime(t1);
	ts2.toPosixTime(t2);
	return t1-t2;
}
/*************************************************************************/
void TimeFrame::setStart(const IA20::Timestamp& tsStart){
	IA20_TRACER;
	if(getDifferenceInSeconds(tsEnd,tsStart) < 0)
			IA20_THROW(IA20::InternalException("tsStart < tsEnd !"));

	this->tsStart=tsStart;
}
/*************************************************************************/
void TimeFrame::setEnd(const IA20::Timestamp& tsEnd){
	IA20_TRACER;

	if(getDifferenceInSeconds(tsEnd,tsStart) < 0)
			IA20_THROW(IA20::InternalException("tsStart < tsEnd !"));

	this->tsEnd=tsEnd;
}
/*************************************************************************/
void TimeFrame::setTimeFrame(const IA20::Timestamp& tsStart,
								const IA20::Timestamp& tsEnd){
	IA20_TRACER;
	if(getDifferenceInSeconds(tsEnd,tsStart) < 0)
		IA20_THROW(IA20::InternalException("tsStart < tsEnd !"));

	this->tsStart=tsStart;
	this->tsEnd=tsEnd;
}
/*************************************************************************/
int TimeFrame::getLengthInSeconds() const{
	IA20_TRACER;
	return getDifferenceInSeconds(tsEnd,tsStart);
}
/*************************************************************************/
bool TimeFrame::isWithin(const IA20::Timestamp& tsTest) const{
	IA20_TRACER;
	IA20_LOG(IA20::LogLevel::INSTANCE.isDetailedInfo(),tsStart.toDateTimeString()<<":"<<tsEnd.toDateTimeString()<<"?"<<tsTest.toDateTimeString())
	return getDifferenceInSeconds(tsTest,tsStart) >= 0 &&
			getDifferenceInSeconds(tsTest,tsEnd)  <= 0;
}
/*************************************************************************/
}
