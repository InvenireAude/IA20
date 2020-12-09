/*
 * File: IA20-CommonLib/src/commonlib/memory/sharable/SharedMemoryFile.h
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
/* IA20_COPYRIGHT */

/* ChangeLog:
 *
 */

#ifndef _IA20_SharedMemoryFile_H_
#define _IA20_SharedMemoryFile_H_

#include <ia20/commonlib/types.h>

namespace IA20{

/*************************************************************************/
/** The SharedMemoryFile class.
 */

class SharedMemoryFile {

public:

	class Descriptor{

	public:

		enum Domain{
				DM_FILE,
				DM_SHARED_MEMORY
			};

			enum OpenMode{
				OM_CREATE_ALWAYS,
				OM_CREATE_IF_NEEDED,
				OM_ATTACH
			};

			enum CloseMode{
				CM_DESTROY,
				CM_DEATTACH
			};

		Descriptor();
		Descriptor(const Descriptor& other);

		void setDomain(Domain iDomain);
		void setOpenMode(OpenMode iOpenMode);
		void setCloseMode(CloseMode iCloseMode);
		void setName(const String& strName);
		void setAddress(void *pAddress);
		void setSize(size_t iSize);

		Domain        getDomain() const;
		OpenMode      getOpenMode() const;
		CloseMode     getCloseMode() const;
		const String& getName() const;
		void*         getAddress() const;
		size_t        getSize() const;

	protected:

		Domain    iDomain;
		OpenMode  iOpenMode;
		CloseMode iCloseMode;

		String strName;

		void *pAddress;

		size_t iSize;
		friend class SharedMemoryFile;
	};


	virtual ~SharedMemoryFile()throw();

	bool         alreadyExisted() const;
	void*        getAddress() const;
	size_t       getSize() const;

	const Descriptor& getDescriptor() const;

	SharedMemoryFile(const Descriptor& aDescriptor);

  static void Sync(const void* pAddress, size_t iDataLength, bool bAsync = true);
  void syncAll(bool bAsync = true);

  static const size_t CPageSize;

protected:

	bool bAlreadyExisted;
	void *pAddress;

	Descriptor aDescriptor;

	int fd;

	void accessFile();
  void accessMemory();

	void mapFile();
};


};
#endif /* _IA20_SharedMemoryFile_H_ */
