/*
 * File: IA20-CommonLib/src/commonlib/tools/MiscTools.h
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
#ifndef MISCTOOLS_H_
#define MISCTOOLS_H_

#include "../types.h"

namespace IA20 {

/** The class. */
 class MiscTools {
public:

	static void   BinarytoHex(unsigned  char *sData, size_t iDataLen, std::string& strOutput);
  static std::string BinarytoHex(unsigned  char *sData, size_t iDataLen);
	static void   HexToBinary(const std::string& strInput,unsigned char *sData, size_t iBufLen);

	static std::string StrToHex(const std::string& strData);
	static std::string StrToPrintableAndHex(const std::string& strData);
	static std::string StrToPrintableOrHex(const std::string& strData);
	static std::string CharToHex(const char cChar);

	static char   HexDigit(int  iValue);
	static char   HexDigit(unsigned char cValue);
	static unsigned char HexValue(char cValue);

	static void   CopyStreams(std::istream& is, std::ostream& os, size_t iLimit = SIZE_MAX);
	static void   CopyStreamsUntilBoundary(std::istream& is, std::ostream& os, const std::string& strBoundary);

	static std::string StreamToString(std::istream& is);

	static void   BinaryToBase64(const unsigned  char *sData, size_t iDataLen, std::string& strOutput);
	static void   StringToBase64(const std::string& strInput, std::string& strOutput);
	static void   Base64ToString(const std::string& strInput, std::string& strOutput);
	static void   Base64ToBinary(const std::string& strInput, unsigned char *sData, size_t iBufferLen, size_t &iDataLen);

	static std::string StrToLower(const std::string& strData);
	static std::string StrToUpper(const std::string& strData);

  static std::string GetUUID();

};

}

#endif /* MISCTOOLS_H_ */
