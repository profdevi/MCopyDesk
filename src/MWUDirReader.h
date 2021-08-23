/*
Copyright (C) 2011-2022, Comine.com ( comine.com@gmail.com )
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.
- The the names of the contributors of this project may not be used to 
  endorse or promote products derived from this software without specific 
  prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
`AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/


//v1.2 copyright Comine.com 20150816U1931
#ifndef MWUDirReader_h
#define MWUDirReader_h

/////////////////////////////////////////////////////
#include "MStdLib.h"

////////////////////////////////////////////////
#if (defined(MSTDLIB_API_WINDOWS) )
#include <windows.h>
#include "MFilePathBuilder.h"
#include "MBuffer.h"
#include "TAutoPtr.h"

///////////////////////////////////////////////
#elif (defined(MSTDLIB_API_LINUX) )
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "MFilePathBuilder.h"
#include "MBuffer.h"
#include "TAutoPtr.h"

#endif // MSTDLIB_API_WINDOWS

//******************************************************
//**  MWUDirReader class
//******************************************************
class MWUDirReader
	{
	////////////////////////////////////////////////
	#if (defined(MSTDLIB_API_WINDOWS) )
	HANDLE mDirHandle;
	TAutoPtr<WIN32_FIND_DATAA> mData;
	bool mDataRead;
	MFilePathBuilder mPathList;
	MBuffer mFullFilePath;
	bool mNoFiles;								//=true if no more files

	///////////////////////////////////////
	#elif (defined(MSTDLIB_API_LINUX) )
	DIR *mDirHandle;
	struct dirent *mData;
	TAutoPtr<struct stat> mStat;
	bool mDataRead;
	MFilePathBuilder mPathList;
	MString mFileName;
	MBuffer mFullFilePath;
	bool mNoFiles;								//=true if no more files

	#endif // MSTDLIB_API_WINDOWS
	
	////////////////////////////////////////////////
	void ClearObject(void);
	
	////////////////////////////////////////////////
	public:
	MWUDirReader(void);
	~MWUDirReader(void);
	bool Create(const char *dirname);
	bool Destroy(void);
	bool Read(void);							// Returns True for each new entry
	const char *GetFileName(void);				// Get a file name
	const char *GetFullFilePath(void);			// Get Full File Path
	bool IsDirectory(void);	
	unsigned long GetFileSizeLow(void);
	unsigned long GetFileSizeHigh(void);
	bool GetCreateTime(time_t &tim);
	bool GetLastAccessTime(time_t &tim);
	bool GetLastWriteTime(time_t &tim);		
	};

#endif // MWUDirReader_h

