/*
 * File: IA20-CommonLib/src/commonlib/tools/MiscTools.h
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
#ifndef MISCTOOLS_H_
#define MISCTOOLS_H_

#include "../types.h"

namespace IA20 {

/** The class. */
 class MiscTools {
public:

	static void   BinarytoHex(const void *pData, size_t iDataLen, String& strOutput);
  static String BinarytoHex(const void *pData, size_t iDataLen);
	static void   HexToBinary(const String& strInput,void *pData, size_t iBufLen);

	static String StrToHex(const String& strData);
	static String StrToPrintableAndHex(const String& strData);
	static String StrToPrintableOrHex(const String& strData);
	static String CharToHex(const char cChar);

	static char   HexDigit(int  iValue);
	static char   HexDigit(unsigned char cValue);
	static unsigned char HexValue(char cValue);

	static void   CopyStreams(std::istream& is, std::ostream& os, size_t iLimit = SIZE_MAX);
	static void   CopyStreamsUntilBoundary(std::istream& is, std::ostream& os, const String& strBoundary);

	static String StreamToString(std::istream& is);

	static void   BinaryToBase64(const unsigned  char *sData, size_t iDataLen, String& strOutput);
	static void   StringToBase64(const String& strInput, String& strOutput);
	static void   Base64ToString(const String& strInput, String& strOutput);
	static void   Base64ToBinary(const String& strInput, unsigned char *sData, size_t iBufferLen, size_t &iDataLen);

	static String StrToLower(const String& strData);
	static String StrToUpper(const String& strData);

  static String GetUUID();

};

}

#endif /* MISCTOOLS_H_ */
