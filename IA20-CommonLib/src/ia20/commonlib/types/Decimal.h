/*
 * File: IA20-CommonLib/src/commonlib/types/Decimal.h
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
#ifndef _IA20_Types_Decimal_H_
#define _IA20_Types_Decimal_H_

#include <ia20/commonlib/types.h>
#include <iostream>

namespace IA20 {

/*************************************************************************/
/** The Decimal class.
 *
 */
class Decimal {
public:

	virtual ~Decimal() throw();

  typedef unsigned short Precision;
  static const Precision C_DefaultPrecision = 0;

  Decimal(const Decimal& d);
	Decimal(Float fValue, Precision iPrecision = C_DefaultPrecision);
  Decimal(long  iValue, Precision iPrecision = C_DefaultPrecision);
  Decimal(int  iValue, Precision iPrecision = C_DefaultPrecision);
	Decimal(const String& strValue, Precision iPrecision = C_DefaultPrecision);

  Precision getPrecision() const;

  friend std::ostream & operator << (std::ostream &out, const Decimal &d);
  friend std::istream & operator >> (std::istream &in,  Decimal &d);

  Decimal& operator=(const Decimal& d1);
  Decimal& operator=(Float fValue);
  Decimal& operator=(long  iValue);
  Decimal& operator=(const String& strValue);

  bool operator==(const Decimal& d1) const;
  bool operator!=(const Decimal& d1) const;
  bool operator<(const Decimal& d1) const;
  bool operator>(const Decimal& d1) const;
  bool operator<=(const Decimal& d1) const;
  bool operator>=(const Decimal& d1) const;

  operator Float() const;
  operator Long() const;
  operator String() const;
  String toString() const;

  Decimal& operator+=(const Decimal& d1);
  Decimal& operator-=(const Decimal& d1);
  Decimal& operator*=(const Decimal& d1);
  Decimal& operator/=(const Decimal& d1);

  friend Decimal operator+(const Decimal& d1, const Decimal& d2);
  friend Decimal operator-(const Decimal& d1, const Decimal& d2);
  friend Decimal operator*(const Decimal& d1, const Decimal& d2);
  friend Decimal operator/(const Decimal& d1, const Decimal& d2);

protected:

  typedef long long DecimalValueHolder;


  void setup(Precision iPrecision);
  void setValue(DecimalValueHolder iValue);
  DecimalValueHolder getRawValue() const;
  DecimalValueHolder getAdjustedRawValue(const Decimal& d) const;

  void parseString(const String& strValue);
  DecimalValueHolder iValue;
};

/*************************************************************************/

}

#endif /* _IA20_Types_Decimal_H_ */
