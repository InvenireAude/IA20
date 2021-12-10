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

	~Converter() throw(){};
	Converter(){};

 static Converter TheInstance;

 Type::Integer convertStringToInteger(const Type::CString csValue);
 void convertIntegerToStream(const Type::Integer iValue, std::ostream& os);

protected:

};

/*************************************************************************/
}
}
}

#endif /* _IA20_DM_Proxy_Converter_H_ */
