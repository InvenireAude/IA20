/*
 * File: IA20-CommonLib/src/commonlib/time/TimeFrame.h
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
#ifndef TIMEFRAME_H_
#define TIMEFRAME_H_

/*
 *
 */
#include "Timestamp.h"

namespace IA20 {

/** The class. */
 class TimeFrame {
public:
	TimeFrame() throw();

	TimeFrame(const TimeFrame& other) throw();

	TimeFrame& operator=(const TimeFrame& other);

	virtual ~TimeFrame() throw();

	void setStart(const IA20::Timestamp& tsStart);
	void setEnd(const IA20::Timestamp& tsEnd);

	void setTimeFrame(const IA20::Timestamp& tsStart, const IA20::Timestamp& tsEnd);

	int getLengthInSeconds() const;

	bool isWithin(const IA20::Timestamp& tsTest) const;

	const Timestamp& getStart() const { return tsStart; };
	const Timestamp& getEnd()   const { return tsEnd;    };

protected:
	Timestamp tsStart;
	Timestamp tsEnd;

	long getDifferenceInSeconds(const Timestamp& ts1, const Timestamp& ts2)const;
};

}

#endif /* TIMEFRAME_H_ */
