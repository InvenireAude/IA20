/*
 * File: Converter.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_DM_Proxy_Converter_H_
#define _IA20_DM_Proxy_Converter_H_

#include <ia20/commonlib/commonlib.h>

#include <ia20/dm/Type.h>
#include <ia20/dm/TypedValue.h>

namespace IA20 {
namespace DM {
namespace Proxy {


/*************************************************************************/
/** The Converter class.
 *
 */
class Converter {
public:

	~Converter() throw();
	Converter();

 static Converter TheInstance;

 void convert(Type::Kind iKindFrom, const TypedValue::Holder& holderFrom,
              Type::Kind iKindTo, TypedValue::Holder& holderTo);

 static void ConvertStringToInteger(const TypedValue::Holder& holderFrom, TypedValue::Holder& holderTo);

protected:

  typedef void (*ConvertFunPtr)(const TypedValue::Holder& holderFrom,
                                TypedValue::Holder& holderTo);

  static const int CTabSize = Type::Kind::CNumberOfTypeKinds*Type::Kind::CNumberOfTypeKinds + 1;
  ConvertFunPtr tConverters[CTabSize];

 static void ConvertByCopy(const TypedValue::Holder& holderFrom, TypedValue::Holder& holderTo);

};

/*************************************************************************/
}
}
}

#endif /* _IA20_DM_Proxy_Converter_H_ */
