/*
 * File: Setter.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_DM_Proxy_Setter_H_
#define _IA20_DM_Proxy_Setter_H_

#include <ia20/commonlib/commonlib.h>

#include <ia20/dm/TypedValue.h>
#include <ia20/dm/Type.h>

namespace IA20 {
namespace DM {
namespace Proxy {

/*************************************************************************/
/** The Setter class.
 *
 */
class Setter {
public:

	~Setter() throw();
	Setter();

  static Setter TheInstance;

  void setInteger(TypedValue* pValue, Type::Integer iValue);
  void setString(TypedValue* pValue,  const String& strValue);

protected:

};
/*************************************************************************/
}
}
}

#endif /* _IA20_DM_Proxy_Setter_H_ */
