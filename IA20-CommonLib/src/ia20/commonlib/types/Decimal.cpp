/*
 * File: IA20-CommonLib/src/commonlib/types/Decimal.cpp
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
#include "Decimal.h"
#include <math.h>
#include <ia20/commonlib/commonlib.h>
#include <algorithm>

namespace IA20 {

#define C_DECIMAL_PREC_BIT  4
#define C_DECIMAL_PREC_MASK 15
#define C_DECIMAL_PREC_OFF (8*sizeof(Decimal::DecimalValueHolder) - C_DECIMAL_PREC_BIT - 1)
#define C_DECIMAL_FLOAT_PREC 5

/*************************************************************************/
Decimal::Decimal(const Decimal& d) {
	IA20_TRACER;
  this->iValue = d.iValue;
}
/*************************************************************************/
Decimal::Decimal(Float fValue, Precision iPrecision) {
	IA20_TRACER;
  if(iPrecision == C_DefaultPrecision)
   iPrecision = C_DECIMAL_FLOAT_PREC;
  setup(iPrecision);
  setValue(fValue * pow(10, iPrecision));
}
/*************************************************************************/
Decimal::Decimal(long iValue, Precision iPrecision) {
	IA20_TRACER;
  setup(iPrecision);
  setValue(iValue * pow(10, iPrecision));
}
/*************************************************************************/
Decimal::Decimal(int iValue, Precision iPrecision) {
	IA20_TRACER;
  setup(iPrecision);
  setValue(iValue * pow(10, iPrecision));
}
/*************************************************************************/
Decimal::Decimal(const String& strValue, Precision iPrecision) {
	IA20_TRACER;
  setup(iPrecision);
  parseString(strValue);
}
/*************************************************************************/
Decimal::~Decimal() {
}
/*************************************************************************/
void Decimal::setup(Precision iPrecision) {
	IA20_TRACER;
  this->iValue = (DecimalValueHolder)iPrecision << C_DECIMAL_PREC_OFF;
}
/*************************************************************************/
void Decimal::setValue(long long iValue) {
	IA20_TRACER;

  Decimal::Precision iPrecision = getPrecision();

  this->iValue = (iValue & ~((DecimalValueHolder)C_DECIMAL_PREC_MASK << C_DECIMAL_PREC_OFF)) |
                  (DecimalValueHolder)iPrecision << C_DECIMAL_PREC_OFF;

}
/*************************************************************************/
long long Decimal::getRawValue() const{
	IA20_TRACER;

  return ( (iValue) & ~((Decimal::DecimalValueHolder)C_DECIMAL_PREC_MASK << C_DECIMAL_PREC_OFF) |
  (iValue >= 0 ? 0 : ((Decimal::DecimalValueHolder)C_DECIMAL_PREC_MASK << C_DECIMAL_PREC_OFF) ));

}
/*************************************************************************/
Decimal::Precision Decimal::getPrecision() const{
	IA20_TRACER;
  return ((iValue >> C_DECIMAL_PREC_OFF) & C_DECIMAL_PREC_MASK);
}
/*************************************************************************/
Decimal::operator Float() const{
  return (Float)getRawValue() / pow(10, getPrecision());
}
/*************************************************************************/
Decimal::operator Long() const{
  IA20_TRACER;

  Long lValue = getRawValue();

  Precision iPrecision = getPrecision();

  while(iPrecision-- > 0){
    lValue = lValue / 10;
  }

  return lValue;
}
/*************************************************************************/
Decimal::operator String() const{
  return toString();
}
/*************************************************************************/
String Decimal::toString() const{
  IA20_TRACER;
  StringStream ssTmp;
  ssTmp<<(*this);
  return ssTmp.str();
}
/*************************************************************************/
Decimal::DecimalValueHolder Decimal::getAdjustedRawValue(const Decimal& d) const{

  int iPrecDiff = getPrecision() - d.getPrecision();
  DecimalValueHolder iValue = d.getRawValue();

  while(iPrecDiff > 0){
    iValue*=10;
    iPrecDiff--;
  }

  while(iPrecDiff < 0){
    iValue/=10;
    iPrecDiff++;
  }

  return iValue;
}
/*************************************************************************/
 Decimal& Decimal::operator+=(const Decimal& d){
  setValue(getRawValue() + getAdjustedRawValue(d));
  return *this;
}
/*************************************************************************/
 Decimal& Decimal::operator-=(const Decimal& d){
  setValue(getRawValue() - getAdjustedRawValue(d));
  return *this;
}
/*************************************************************************/
 Decimal& Decimal::operator*=(const Decimal& d){
  DecimalValueHolder iValue = getRawValue() * d.getRawValue();

  int iPrecision = d.getPrecision();
  while(iPrecision-- > 0){
    iValue = iValue / 10;
  }
  setValue(iValue);
  return *this;
}
/*************************************************************************/
 Decimal& Decimal::operator/=(const Decimal& d){
  DecimalValueHolder iValue = getRawValue() / d.getRawValue();

  int iPrecision = d.getPrecision();

  while(iPrecision-- > 0){
    iValue = iValue * 10L;
  }

  setValue(iValue);

  return *this;
}
/*************************************************************************/
Decimal operator+(const Decimal& d1, const Decimal& d2){
  Decimal d(d1);
  d += d2;
  return d;
}
/*************************************************************************/
Decimal operator-(const Decimal& d1, const Decimal& d2){
  Decimal d(d1);
  d -= d2;
  return d;
}
/*************************************************************************/
Decimal operator*(const Decimal& d1, const Decimal& d2){
  Decimal d(d1);
  d *= d2;
  return d;
}
/*************************************************************************/
Decimal operator/(const Decimal& d1, const Decimal& d2){
  Decimal d(d1);
  d /= d2;
  return d;
}
/*************************************************************************/
Decimal& Decimal::operator=(const Decimal& d1){
  this->iValue = d1.iValue;
  return *this;
}
/*************************************************************************/
Decimal& Decimal::operator=(Float fValue){
  setup(C_DECIMAL_FLOAT_PREC);
  setValue(fValue * pow(10, C_DECIMAL_FLOAT_PREC));
  return *this;
}
/*************************************************************************/
Decimal& Decimal::operator=(long  iValue){
  setup(0);
  setValue(iValue * pow(10, getPrecision()));
  return *this;
}
/*************************************************************************/
Decimal& Decimal::operator=(const String& strValue){
  parseString(strValue);
  return *this;
}
/*************************************************************************/
void Decimal::parseString(const String& strValue){

  StringStream ssTmp(strValue);

  try {

    ssTmp>>(*this);

  }catch(BadUsageException& e){
    IA20_THROW(BadUsageException(e.getInfo() + "["+strValue+"]"));
  }

  while(!ssTmp.eof() && isblank(ssTmp.get()))
    /* skip() */;

  if(!ssTmp.eof()){
    IA20_THROW(BadUsageException("Error data in a decimal value. ["+strValue+"]"));
  }

}
/*************************************************************************/
bool Decimal::operator==(const Decimal& d)const{
  return getRawValue() == getAdjustedRawValue(d);
}
/*************************************************************************/
bool Decimal::operator!=(const Decimal& d)const{
  return getRawValue() != getAdjustedRawValue(d);
}
/*************************************************************************/
bool Decimal::operator<(const Decimal& d)const{
  return getRawValue() < getAdjustedRawValue(d);
}
/*************************************************************************/
bool Decimal::operator>(const Decimal& d)const{
  return getRawValue() > getAdjustedRawValue(d);
}
/*************************************************************************/
bool Decimal::operator<=(const Decimal& d)const{
  return getRawValue() <= getAdjustedRawValue(d);
}
/*************************************************************************/
bool Decimal::operator>=(const Decimal& d)const{
  return getRawValue() >= getAdjustedRawValue(d);
}
/*************************************************************************/

std::ostream & operator << (std::ostream &out, const Decimal &d)
{
  long long iValue = d.getRawValue();
  Decimal::Precision iPrecision = d.getPrecision();

  bool bNegative = iValue < 0;

  if(bNegative) {
    iValue = -iValue;
  }

  String strTmp;
  strTmp.reserve(256);

  bool bFirst = true;
  while(bFirst || iValue != 0 || iPrecision > 0){
    bFirst = false;
    strTmp += ('0' + iValue % 10);
    iValue /= 10;
    if(iPrecision > 0 && --iPrecision == 0){
      strTmp += ".";
      if(iValue == 0){
        strTmp+="0";
      }
    }
  }

  if(bNegative){
    strTmp+="-";
  }

  std::reverse(strTmp.begin(),strTmp.end());
  out<<strTmp;

  return out;
}

/*************************************************************************/

std::istream & operator >> (std::istream &in,  Decimal &d)
{

  int c;
  int iSign = 1;
  bool bFirst = true;
  bool bDecrementPrecision = false;
  bool bStop = false;
  bool bHasData = false;
  long long iValue = 0;
  bool bPrecisionFromInput=false;

  Decimal::Precision iPrecision = d.getPrecision();

  if(iPrecision == Decimal::C_DefaultPrecision) {
    bPrecisionFromInput = true;
    iPrecision = C_DECIMAL_PREC_MASK - 1;
  }

  while(!bStop && (c = in.get()) && in.good()){

    if( isdigit(c) ){
        bFirst = false;
        bHasData = true;
      if( !(bDecrementPrecision && iPrecision == 0) ){
        iValue = iValue * 10 + (c - '0');
      }

      if(bDecrementPrecision && iPrecision){
        iPrecision--;
      }

    }else if(!bDecrementPrecision && c == '.') {

      bDecrementPrecision = true;

    } else if( bFirst && c == '-'){

        iSign = -1;
        bFirst = false;

    }else if(bFirst && c == '+'){

        bFirst = false;
    } else if(bFirst && isblank(c)){
    } else {
      bStop = true;
    }
  }

  if((in.good() && !isblank(c)) || ! bHasData){
    IA20_THROW(BadUsageException("Error in a decimal value."));
  }

  if(bPrecisionFromInput) {
    d.setup(C_DECIMAL_PREC_MASK - 1 - iPrecision);
    iPrecision = 0;
  }

  while(iPrecision-- > 0){
    iValue = iValue * 10;
  }

  if(in.good()){
    in.unget();
  }

  d.setValue(iValue * iSign);

  return in;
}
/*************************************************************************/

}

