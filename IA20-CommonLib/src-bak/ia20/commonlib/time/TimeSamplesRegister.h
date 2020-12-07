/*
 * File: IA20-CommonLib/src/commonlib/time/TimeSamplesRegister.h
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
#ifndef _IA20_TimeSamplesRegister_H_
#define _IA20_TimeSamplesRegister_H_

#include <time.h>
#include <ostream>
#include <climits>

namespace IA20{
/*************************************************************************/
class TimeSample {
public:

	TimeSample(bool bStart = false){
		if(bStart)
			start();
		else
			tsStart.tv_sec=0L;
	}

	void start();

	long getSample()const;

protected:

	struct timespec tsStart;
};
/*************************************************************************/
struct TimeSamplesResults{

	static const int COF_MicroSec = 1;
	static const int COF_MiliSec = 1000;
	static const int COF_Seconds = 1000000;

	TimeSamplesResults(int iOutputFactor = 1000):
		iTotal(0L),iMin(0),iMax(0L),iNumSamples(0L),
		iOutputFactor(iOutputFactor){};

	long iTotal;
	long iMin;
	long iMax;
	long iNumSamples;

	int iOutputFactor;

	TimeSamplesResults(const TimeSamplesResults& other):
		iTotal(other.iTotal),iMin(other.iMin),iMax(other.iMax),iNumSamples(other.iNumSamples),
		iOutputFactor(other.iOutputFactor){};

	void reset(){
		iTotal=0;
		iMin=0;
		iMax=0;
		iNumSamples=0;
	}


	inline TimeSamplesResults& operator=(const TimeSamplesResults& other){
		this->iMin=other.iMin;
		this->iMax=other.iMax;
		this->iTotal=other.iTotal;
		this->iNumSamples=other.iNumSamples;
		this->iOutputFactor = other.iOutputFactor;
		return *this;
	}

	inline void addSample(long iValue){
		iValue*=1000;

		if(iValue >iMax)
			iMax=iValue;

		if(iValue < iMin || iNumSamples==0)
			iMin=iValue;

		iTotal+=iValue;
		iNumSamples++;
	}

	inline void addSample(const TimeSample& ts){

		long iTimeElapsed=ts.getSample();

		if(iTimeElapsed >iMax)
			iMax=iTimeElapsed;

		if(iTimeElapsed < iMin || iNumSamples==0)
			iMin=iTimeElapsed;

		iTotal+=iTimeElapsed;
		iNumSamples++;

	}

	friend std::ostream& operator<<(std::ostream& os, const TimeSamplesResults& r){
		//TODO (L) c++ way
		char sBuff[256];
		sprintf(sBuff,"min = %10.2f\t max = %10.2f\tavg = %10.2f\t total = %10.2f\tnum = %10ld",
				1.0*r.iMin/r.iOutputFactor,
				1.0*r.iMax/r.iOutputFactor,
				r.iNumSamples > 0 ? 1.0*r.iTotal/r.iNumSamples/r.iOutputFactor : 0.0,
				1.0*r.iTotal/r.iOutputFactor,
				r.iNumSamples);

		os<<sBuff;
		return os;
	}
};
/*************************************************************************/
class AutoTimeSample : public TimeSample {
	public:
	AutoTimeSample(TimeSamplesResults& tsr, bool bStart=true):tsr(tsr),TimeSample(bStart){}

	~AutoTimeSample(){
		tsr.addSample(*this);
	}

	TimeSamplesResults& tsr;
};
/*************************************************************************/
//TODO class TSRW : public TimeSamplesResults {
//public:
//~TSRW(){ std::cerr<<(*this)<<std::endl; }
//};

}

#endif /* _IA20_TimeSamplesRegister_H_ */
