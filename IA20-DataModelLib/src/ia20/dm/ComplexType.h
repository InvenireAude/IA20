/*
 * File: ComplexType.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_DM_ComplexType_H_
#define _IA20_DM_ComplexType_H_

#include <ia20/commonlib/commonlib.h>

#include <ia20/dm/exception/Exception.h>

#include <vector>
#include <memory>

#include "Type.h"
#include "Property.h"

namespace IA20 {
namespace DM {

class DataObject;
/*************************************************************************/
/** The ComplexType class.
 *
 */
class ComplexType : public Type {
public:

	~ComplexType() throw();
	ComplexType(const String& strName, const Type* pParent = NULL);

  virtual const ComplexType* asComplexType()const;

  inline bool isRoot()const{
    return pParent == NULL;
  }

  const Type* getParent()const{
    return pParent;
  }

  void defineProperty(const Type* pType, const String& strName);

  inline unsigned int getNumProperties()const{
    return tabProperties.size();
  }

  typedef std::vector< const Property* > PropertiesArray;

  inline const Property* getProperty(unsigned int iIdx)const{
      //IA20_LOG(true, "Property: "<<tabProperties[iIdx]->getName()<<", idx:"<<tabProperties[iIdx]->getIdx());
      return tabProperties[iIdx];
  };

  inline const Property* getProperty(const String& strName)const{
      for(auto p : tabProperties){
        if(p->getName().compare(strName) == 0)
          return p;
      }
      IA20_THROW(DM::Exception("Property not found: [")<<strName<<"]");
  };

  inline const PropertiesArray* getProperties()const{
    return &tabProperties;
  }

 virtual DataObject* create(DataObject *pParent = NULL)const;

protected:

  typedef std::vector< std::unique_ptr<Property> > LocalPropertiesArray;

  LocalPropertiesArray tabLocalProperties;
  PropertiesArray      tabProperties;

  const ComplexType* pParent;
  unsigned int iPropertyOffset;
};

/*************************************************************************/
}
}

#endif /* _IA20_DM_ComplexType_H_ */
