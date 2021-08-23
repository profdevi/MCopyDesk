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


//v1.2 copyright Comine.com 20150816U1930
#include "MStdLib.h"
#include "MBuffer.h"
#include "MString.h"
#include "TQueueArray.h"
#include "MStringSplitter.h"
#include "MFilePathBuilder.h"


/*
	Implementation Notes:
	1.  container will never contain . or .. path elements.
	2.  There is never a trailing / in file path.  This may create problems
		such as c: on windows.  But, linux can have directories with colon.
*/

//******************************************************
//**  Module Elements
//******************************************************
static const int GMaxDepth=30;				// Max Directory Depth
static const int GMaxDirectoryLength=1000;	// Max Directory Size

/////////////////////////////////////////////////////
#if(defined(MSTDLIB_API_WINDOWS) )
static bool GIsWindowsDriveLetter(const char *element)
	{
	if(*element==0) { return false; }
	if(MStdIsAlpha(*element)==false) { return false; }
	if(element[1]==0) { return false; }
	if(element[1]!=':') { return false; }
	if(element[2]==0) { return true; }
	return false; 
	}

/////////////////////////////////
#else
static bool GIsWindowsDriveLetter(const char *element)
	{
	return false;
	}

#endif	// MSTDLIB_API_WINDOWS

//******************************************************
//**  MFilePathBuilder class
//******************************************************
void MFilePathBuilder::ClearObject(void)
	{
	}


////////////////////////////////////////////////
MFilePathBuilder::MFilePathBuilder(bool init)
	{
	ClearObject();
	if(init==true && Create()==false)
		{
		return;
		}
	}


///////////////////////////////////////////////
MFilePathBuilder::MFilePathBuilder(const char *dirpath)
	{
	if(Create(dirpath)==false)
		{
		return;
		}
	}


////////////////////////////////////////////////
MFilePathBuilder::~MFilePathBuilder(void)
	{  Destroy();  }


////////////////////////////////////////////////
bool MFilePathBuilder::Create(void)
	{
	Destroy();

	if(mList.Create(GMaxDepth)==false)
		{
		Destroy();
		return false;
		}

	return true;
	}

////////////////////////////////////////////////
bool MFilePathBuilder::Create(const char *path)
	{
	if(Create()==false) { return false; }

	MStringSplitter splitter;
	if(splitter.Create(path,"\\/")==false)
		{
		Destroy();
		return false;
		}

	// Copy into the mList
	const int count = splitter.GetCount();
	for(int i=0;i<count;++i)
		{
		const char *element=splitter.Get(i);
		if(MStdStrCmp(element,".")==0 || MStdStrCmp(element,"..")==0 )
			{
			Destroy();
			return false;
			}

		MString *newbackitem;
		newbackitem=mList.EnqueueBack();
		if(newbackitem==NULL || newbackitem->Create(splitter.Get(i) )==false)
			{
			Destroy();
			return false;
			}
		}

	return true;
	}


////////////////////////////////////////////////
bool MFilePathBuilder::Destroy(void)
	{
	mList.Destroy();
	ClearObject();
	return true;
	}


////////////////////////////////////////////////
const char *MFilePathBuilder::Get(int index)
	{
	if(index<0 || index>=mList.GetLength() )
		{
		return NULL;
		}

	return mList.Get(index)->Get() ;
	}


////////////////////////////////////////////////
int MFilePathBuilder::GetElementCount(void)
	{
	return mList.GetLength();
	}


////////////////////////////////////////////////
bool MFilePathBuilder::GetFullPath(MBuffer &buffer)
	{
	if(buffer.Create(GMaxDirectoryLength)==false)
		{
		return false;
		}

	// Init Buffer
	buffer.SetString("");

	const int length=mList.GetLength();

	if(length==0)
		{
		return true;
		}

	// Dont Append if first element is windows drive letter
	const char *element=mList.Get(0)->Get();
	if(GIsWindowsDriveLetter(element)==false)
		{
		buffer.StringAppend("/");
		}
	
	// Add the first item
	buffer.StringAppend(element);

	for(int i=1;i<length;++i)
		{
		buffer.StringAppend("/");
		buffer.StringAppend(mList.Get(i)->Get() );
		}

	return true;
	}


////////////////////////////////////////////////
bool MFilePathBuilder::Push(const char *element)		// Push Element into list
	{
	if(element==NULL || *element==0)
		{
		return false;
		}

	MStringSplitter splitter;
	if(splitter.Create(element,"\\/")==false)
		{
		return false;
		}

	const int splitcount=splitter.GetCount();

	// Check for . and ..
	int i;
	for(i=0;i<splitcount;++i)
		{
		const char *element=splitter.Get(i);
		if(MStdStrCmp(element,".")==0 || MStdStrCmp(element,"..")==0 )
			{
			return false;
			}
		}
	
	for(i=0;i<splitcount;++i)
		{
		// Enqueue new item
		MString *newitem=mList.EnqueueBack();
		if(newitem==NULL)
			{
			return false;
			}

		// Copy Name into new queued item
		if(newitem->Create(splitter.Get(i) )==false)
			{
			mList.DequeueBack();
			return false;
			}
		}

	return true;
	}


////////////////////////////////////////////////
bool MFilePathBuilder::Push(MFilePathBuilder &paths)	// Push More Elements at end
	{
	const int count=paths.GetElementCount();
	int i;
	for(i=0;i<count;++i)
		{
		MString *newitem=mList.EnqueueBack();
		if(newitem==NULL)
			{
			return false;
			}

		if(newitem->Create(paths.Get(i))==false)
			{
			mList.DequeueBack();
			return false;
			}
		}

	return true;
	}


////////////////////////////////////////////////
bool MFilePathBuilder::Pop(void)						// Pop Element off list
	{
	if(mList.DequeueBack()==false)
		{
		return false;
		}

	return true;
	}


////////////////////////////////////////////////
bool MFilePathBuilder::Pop(int cnt)				// Pop Till Count elements exist
	{

	while(mList.GetLength()>cnt)
		{
		if(Pop()==false) { return false; }
		}

	return true;
	}

////////////////////////////////////////////////
const char *MFilePathBuilder::GetTop(void)			// Get Last Element
	{
	const int length=mList.GetLength();
	if(length<=0)
		{
		return NULL;
		}

	return mList.Get(length-1)->Get();
	}


////////////////////////////////////////////////
bool MFilePathBuilder::IsSamePath(MFilePathBuilder &lst)	// =true if paths are same
	{
	MBuffer buf1,buf2;
	if(lst.GetFullPath(buf1)==false || GetFullPath(buf2)==false)
		{
		return false;
		}

	if(MStdStrCmp(buf1.GetBuffer(),buf2.GetBuffer())!=0) { return false; }

	return true;
	}


//////////////////////////////////////////////////
bool MFilePathBuilder::IsSamePath(const char *path)		// =true if paths are same
	{
	if(path==NULL)
		{
		return false;
		}

	MFilePathBuilder newpath;
	if(newpath.Create(path)==false)
		{
		return false;
		}

	return IsSamePath(newpath);
	}


