/*
 * File: DataFactory.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_DM_DataFactory_H_
#define _IA20_DM_DataFactory_H_

#include <ia20/commonlib/commonlib.h>

#include <ia20/dm/tools/Hash.h>

#include <unordered_map>
#include <memory>

#include "TypeFactory.h"

namespace IA20 {
namespace DM {

/*************************************************************************/
/** The DataFactory class.
 *
 */

class Type;
class Namespace;

class DataFactory{
public:

	virtual ~DataFactory() throw();

	DataFactory(const DataFactory* pParent = TheDefaultInstance);

  Namespace* createNamespace(const String& strNamespace);

  Type* createType(const String& strNamespace, Type::Kind iKind, const String& strType, const Type* pBaseType = NULL);

  const Namespace* getNamespace(const String& strNamespace)const{
    return const_cast<DataFactory*>(this)->getNamespace(strNamespace);
  }

  const Type* getType(const String& strNamespace, const String& strType)const;

  // Thread safety is up to the caller !!!
  inline static const DataFactory* GetDefault(){

    if(!TheDefaultInstance){
      std::unique_ptr<DataFactory> ptrTemp(new DataFactory(NULL));
      ptrTemp->setupDefaultTypes();
      TheDefaultInstance = ptrTemp.release();
    }

    return TheDefaultInstance;
  }

protected:

  static DataFactory* TheDefaultInstance;

  const DataFactory* pParent;

  typedef std::unordered_map<String, std::unique_ptr<Namespace>, Tools::Hash::StringHash > NamespaceMap;
  NamespaceMap hmNamespaces;

  typedef std::unordered_map<Tools::Hash::KeyPairType, Type*, Tools::Hash::StringRefPairHash> QuickLookUpMap;
  QuickLookUpMap hmQuickLookUp;

  Namespace* getNamespace(const String& strNamespace);
  Type*      getTypeImpl(const String& strNamespace, const String& strType);


  void setupDefaultTypes();

};

/*************************************************************************/
}
}

#endif /* _IA20_DM_DataFactory_H_ */
