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
#include "MStdLib.h"

/////////////////////////////////////////
#if (defined(MSTDLIB_API_WINDOWS) )
#include <Windows.h>
#include <time.h>
#include "MBuffer.h"
#include "MFileOps.h"
#include "TAutoPtr.h"
#include "MFilePathBuilder.h"

/////////////////////////////////////////
#elif (defined(MSTDLIB_API_LINUX) )
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "MBuffer.h"
#include "MFileOps.h"
#include "MFilePathBuilder.h"
#include "MString.h"


#endif // MSTDLIB_API_WINDOWS

/////////////////////////////////////////
#include "MWUDirReader.h"


//////////////////////////////////////////////////
static const int GMaxDirPathLength=2000;


#if (defined(MSTDLIB_API_WINDOWS) )
//******************************************************
//**  Module Elements
//******************************************************
static bool GReducePath(const char *passedarg,MBuffer &outreducedpath)
	{
	if(outreducedpath.Create(1000)==false)
		{
		return false;
		}

	outreducedpath.SetString(passedarg);
	int length=outreducedpath.GetStringLength();
	int i;
	for(i=length-1;i>=0;--i)
		{
		if(outreducedpath[i]=='/' || outreducedpath[i]=='\\')
			{
			outreducedpath[i]=0;
			return true;
			}
		}

	outreducedpath[0]=0;
	return true;
	}


static time_t GConvert(const FILETIME &ft)
	{
	// Following Code is copied from :
	//   http://ix-notes.blogspot.com/2008/03/filetime-to-timet.html
	ULONGLONG ull = reinterpret_cast<const ULONGLONG&>(ft);
	ull -= 116444736000000000;
	ull /= 10000000;
	MStdAssert(ull < ULONG_MAX);
	return static_cast<time_t>(ull);
	}


//******************************************************
//**  MWUDirReader class
//******************************************************
void MWUDirReader::ClearObject(void)
	{
	mDirHandle=INVALID_HANDLE_VALUE;
	mDataRead=false;
	mNoFiles=false;
	}


////////////////////////////////////////////////
MWUDirReader::MWUDirReader(void)
	{  ClearObject();  }


////////////////////////////////////////////////
MWUDirReader::~MWUDirReader(void)
	{  Destroy();  }


////////////////////////////////////////////////
bool MWUDirReader::Create(const char *dirpath)
	{
	Destroy();

	// Get Absolute Path directory
	MFileOps fileops(true);
	MString abspath;
	if(fileops.GetAbsolutePath(dirpath,abspath)==false)
		{
		Destroy();
		return false;
		}

	// First Check if this is a directory
	if(fileops.IsDirectory(abspath.Get() )==false)
		{
		Destroy();
		return false;
		}

	if(mPathList.Create(abspath.Get() )==false)
		{
		Destroy();
		return false;
		}

	// Allocate Win32 API structure
	if(mData.Create()==false)
		{
		Destroy();
		return false;
		}

	MStdMemZero(mData.Get(),sizeof(WIN32_FIND_DATAA) );


	// Append all the files with *.* for windows
	MBuffer bufdirpath(GMaxDirPathLength);
	bufdirpath.SetString(abspath.Get() );
	
	if(MStdStrEnds(bufdirpath.GetBuffer(),"/")==false && MStdStrEnds(bufdirpath.GetBuffer(),"\\")==false)
		{
		bufdirpath.StringAppend("/");
		}

	bufdirpath.StringAppend("*.*");

	// open the files specified
	mDirHandle=FindFirstFileA(bufdirpath.GetBuffer(),mData.Get() );
	if(mDirHandle==INVALID_HANDLE_VALUE)
		{
		DWORD error=GetLastError();
		if(error==2)
			{
			//=We have no files
			mNoFiles=true;

			// Init Dummy Vars
			if(mFullFilePath.Create(10)==false)
				{
				Destroy();
				return false;
				}

			if(mFullFilePath.SetString("")==false)
				{
				Destroy();
				return false;
				}

			return true;
			}

		Destroy();
		return false;
		}

	mDataRead=true;
	mNoFiles=false;


	if(MStdStrCmp(".",mData->cFileName)==0 || MStdStrCmp("..",mData->cFileName)==0)
		{
		if(mPathList.Push("....")==false)
			{
			Destroy();
			return false;
			}
		}
	else if(mPathList.Push(mData->cFileName)==false)
		{
		Destroy();
		return false;
		}

	if(mPathList.GetFullPath(mFullFilePath)==false)
		{
		Destroy();
		return false;
		}
	
	mDataRead=true;
	mNoFiles=false;
	return true;
	}


////////////////////////////////////////////////
bool MWUDirReader::Destroy(void)
	{
	mData.Destroy();
	mPathList.Destroy();
	mFullFilePath.Destroy();
	
	if(mDirHandle!=INVALID_HANDLE_VALUE)
		{
		FindClose(mDirHandle);
		mDirHandle=INVALID_HANDLE_VALUE;
		}

	ClearObject();
	return true;
	}


///////////////////////////////////////////////
bool MWUDirReader::Read(void)
	{
	if(mNoFiles==true) {  return false; }

	for(;;)
		{
		if(mDataRead==true)
			{
			if(MStdStrCmp(mData->cFileName,".")==0 || MStdStrCmp(mData->cFileName,"..")==0 )
				{
				mDataRead=false;
				continue;
				}

			mPathList.Pop();
			mPathList.Push(mData->cFileName);
			mPathList.GetFullPath(mFullFilePath);
			mDataRead=false;
			return true;
			}
		else
			{
			if(FindNextFileA(mDirHandle,mData.Get())==0)
				{
				mPathList.Create("");
				MStdMemZero(mData.Get(),sizeof(WIN32_FIND_DATAA) );
				mNoFiles=true;
				mFullFilePath.SetString("");
				return false;
				}

			mDataRead=true;
			}
		}
	}


///////////////////////////////////////////////
const char *MWUDirReader::GetFileName(void)
	{
	return mData->cFileName;
	}


///////////////////////////////////////////////
const char *MWUDirReader::GetFullFilePath(void)
	{
	return mFullFilePath.GetBuffer();
	}


///////////////////////////////////////////////
bool MWUDirReader::IsDirectory(void)
	{
	if(mData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) { return true; }
	return false;
	}


///////////////////////////////////////////////
unsigned long MWUDirReader::GetFileSizeLow(void)
	{
	return mData->nFileSizeLow;
	}



///////////////////////////////////////////////
unsigned long MWUDirReader::GetFileSizeHigh(void)
	{
	return mData->nFileSizeHigh;
	}


///////////////////////////////////////////////
bool MWUDirReader::GetCreateTime(time_t &tim)
	{
	tim=GConvert(mData->ftCreationTime);
	return true;
	}


///////////////////////////////////////////////
bool MWUDirReader::GetLastAccessTime(time_t &tim)
	{
	tim=GConvert(mData->ftLastAccessTime);
	return true;
	}


///////////////////////////////////////////////
bool MWUDirReader::GetLastWriteTime(time_t &tim)
	{
	tim=GConvert(mData->ftLastWriteTime);
	return true;
	}




//////////////////////////////////////////////////////////
#elif (defined(MSTDLIB_API_LINUX) )
//******************************************************
//**  Unix Implementation
//******************************************************

//******************************************************
//**  MWUDirReader class
//******************************************************
void MWUDirReader::ClearObject(void)
	{
	mDirHandle=NULL;
	mData=NULL;
	mDataRead=false;
	mNoFiles=false;
	}


/////////////////////////////////////////////////
MWUDirReader::MWUDirReader(void)
	{  ClearObject();  }


////////////////////////////////////////////////
MWUDirReader::~MWUDirReader(void)
	{  Destroy();  }


////////////////////////////////////////////////
bool MWUDirReader::Create(const char *dirpath)
	{
	Destroy();

	MFileOps fileops(true);
	if(fileops.IsDirectory(dirpath)==false)
		{
		Destroy();
		return false;
		}

	MString absdirpath;
	if(fileops.GetAbsolutePath(dirpath,absdirpath)==false)
	    {
	    Destroy();
	    return false;
	    }

	mDirHandle=opendir(absdirpath.Get() );
	if(mDirHandle==NULL)
		{
		Destroy();
		return false;
		}

	if(mPathList.Create(absdirpath.Get() )==false)
	    {
		Destroy();
		return false;
	    }

	if(mFileName.Create("")==false)
		{
		return false;
		}

	if(mStat.Create()==false)
		{
		Destroy();
		return false;
		}

	mDataRead=false;
	return true;
	}


////////////////////////////////////////////////
bool MWUDirReader::Destroy(void)
	{
	mPathList.Destroy();
	mFullFilePath.Destroy();
	mFileName.Destroy();
	mStat.Destroy();
	
	if(mDirHandle!=NULL)
		{
		closedir(mDirHandle);
		mDirHandle=NULL;
		}

	ClearObject();
	return true;
	}


////////////////////////////////////////////////
bool MWUDirReader::Read(void)
	{
	if(mNoFiles==true)
		{  return false; }

	for(;;)
		{
		mData = readdir(mDirHandle);
		if(mData==NULL)
			{
			mNoFiles=true;
			return false;
			}
		else if(MStdStrCmp(mData->d_name,".")==0)
			{
			continue;
			}
		else if(MStdStrCmp(mData->d_name,"..")==0)
			{
			continue;
			}
		else
			{
			break;
			}
		}

	// Push the read filename into mPathList
	if(mPathList.Push(mData->d_name)==false )
		{
		mNoFiles=true;
		return false;
		}

	if(mPathList.GetFullPath(mFullFilePath)==false)
		{
		mNoFiles=true;
		return false;
		}

	// Remove the file name element
	mPathList.Pop();

	if(mFileName.Create(mData->d_name)==false)
		{
		mNoFiles=true;
		return false;
		}

	// Process the stat info
	if(lstat(mFullFilePath.GetBuffer(),mStat.Get() )<0)
		{
		MStdMemZero(mStat.Get(),sizeof(struct stat) );
		mNoFiles=true;
		return false;
		}

	return true;
	}


/////////////////////////////////////////////////
const char *MWUDirReader::GetFileName(void)
	{
	return mFileName.Get();
	}


///////////////////////////////////////////////
const char *MWUDirReader::GetFullFilePath(void)
	{
	return mFullFilePath.GetBuffer();
	}


///////////////////////////////////////////////
bool MWUDirReader::IsDirectory(void)
	{
	if(S_ISDIR(mStat->st_mode)!=0)
		{
		return true;
		}

	return false;
	}


///////////////////////////////////////////////
unsigned long MWUDirReader::GetFileSizeLow(void)
	{
	return (unsigned long) mStat->st_size;
	}


///////////////////////////////////////////////
unsigned long MWUDirReader::GetFileSizeHigh(void)
	{
	return 0;
	}


///////////////////////////////////////////////
bool MWUDirReader::GetCreateTime(time_t &tim)
	{
	tim=mStat->st_ctime;
	return true;
	}


///////////////////////////////////////////////
bool MWUDirReader::GetLastAccessTime(time_t &tim)
	{
	tim=mStat->st_atime;
	return true;
	}


///////////////////////////////////////////////
bool MWUDirReader::GetLastWriteTime(time_t &tim)
	{
	tim=mStat->st_mtime;
	return true;
	}




#endif // MSTDLIB_API_WINDOWS
