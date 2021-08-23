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


//v1.11 copyright Comine.com 20150813R1241
#include "MStdLib.h"
#include "MBuffer.h"
#include "MStringBuffer.h"
#include "MPathNameList.h"


//******************************************************
//**  Module Elements
//******************************************************
static const int GMaxDepth=20;				// Max Directory Depth
static const int GMaxDirectoryLength=1000;	// Max Directory Size

/////////////////////////////////////////////////
static bool GIsAlpha(char ch)
	{
	if(ch>='A' && ch<='Z') {  return true; }
	if(ch>='a' && ch<='z') { return true; }
	return false;
	}


/////////////////////////////////////////////////
static bool GConvertToCanonicalPath(char *path)
	{
	MStdAssert(path!=NULL);
	char *p;
	for(p=path;*p!=0;++p)
		{
		if(*p=='\\') { *p='/'; }
		}

	return true;
	}

//******************************************************
//**  MPathNameList class
//******************************************************
void MPathNameList::ClearObject(void)
	{
	mDriveLetter=0;
	mList=NULL;
	mMaxLen=0;
	mLen=0;
	}


////////////////////////////////////////////////
bool MPathNameList::InitArray(const char *path)
	{
	if(path==NULL) { return false; }
	if(*path==0) { return false; }
	
	int index;
	if(GIsAlpha(path[0])==true && path[1]==':')
		{  mDriveLetter=path[0];  index=2;  }
	else
		{  index=0;  }

	MStringBuffer buffer;
	if(buffer.Create(1024)==false)
		{
		return false;
		}

	buffer.Clear();
	
	int i;
	for(i=index;path[i]!=0;++i)
		{
		if(path[i]=='/' || path[i]=='\\')
			{
			if(buffer.GetStringLength()>0)
				{
				if(Push(buffer.Get())==false) { return false; }

				buffer.Clear();
				}
			}
		else
			{
			if(buffer.Add(path[i])==false)
				{
				return false;
				}
			}
		}

	// Now Check if Buffer still contains any element
	if(buffer.GetStringLength()>0)
		{
		if(Push(buffer.Get())==false)
			{ return false; }
		}

	return true;
	}


////////////////////////////////////////////////
MPathNameList::MPathNameList(bool init)
	{
	ClearObject();
	if(init==true && Create()==false)
		{
		return;
		}
	}


////////////////////////////////////////////////
MPathNameList::~MPathNameList(void)
	{  Destroy();  }


////////////////////////////////////////////////
bool MPathNameList::Create(void)
	{
	Destroy();

	// allocate space for array
	mList=new(std::nothrow) char *[GMaxDepth];
	if(mList==NULL)
		{
		Destroy();  return false;
		}

	int i;
	mMaxLen=GMaxDepth;
	for(i=0;i<mMaxLen;++i) {  mList[i]=NULL;  }

	return true;
	}

////////////////////////////////////////////////
bool MPathNameList::Create(const char *path)
	{
	if(Create()==false) { return false; }

	InitArray(path);

	return true;
	}


////////////////////////////////////////////////
bool MPathNameList::Destroy(void)
	{
	if(mList!=NULL)
		{
		for(int i=0;i<mMaxLen;++i)
			{
			if(mList[i]!=NULL)
				{
				delete[] mList[i];
				}
			}

		delete[] mList;
		}

	ClearObject();
	return true;
	}


////////////////////////////////////////////////
const char *MPathNameList::Get(int index)
	{
	if(index<0 || index>=mLen)
		{
		return NULL;
		}

	return mList[index];
	}


////////////////////////////////////////////////
int MPathNameList::GetElementCount(void)
	{
	return mLen;
	}


////////////////////////////////////////////////
bool MPathNameList::GetFullPath(MBuffer &buffer)
	{
	if(buffer.Create(GMaxDirectoryLength)==false)
		{
		return false;
		}

	if(mDriveLetter!=0)
		{
		MStdSPrintf(buffer,buffer.GetSize(),"%c:",mDriveLetter);
		}
	else
		{
		buffer.SetString("");
		}

	// Return / root if no elements at all
	if(mLen==0)
		{
		buffer.StringAppend("/");
		return true;
		}

	for(int i=0;i<mLen;++i)
		{
		buffer.StringAppend("/");
		buffer.StringAppend(mList[i]);
		}

	return true;
	}


////////////////////////////////////////////////
bool MPathNameList::Push(const char *element)		// Push Element into list
	{
	if(element==NULL || *element==0)
		{
		return false;
		}

	// Same Directory
	if(MStdStrCmp(element,".")==0) { return true; }

	// Move To Parent
	if(MStdStrCmp(element,"..")==0)
		{
		if(mLen>0)
			{
			if(Pop()==false) { return false; }
			}
		return true;
		}

	if(mLen>=mMaxLen)
		{
		return false;
		}

	// Allocate space for fresh string
	char *newelm;
	const int stringlength=MStdStrLen(element)+1;
	newelm=new(std::nothrow) char[stringlength];
	if(newelm==NULL)
		{
		return false;
		}

	MStdStrCpy(newelm,element);
	GConvertToCanonicalPath(newelm);
	mList[mLen]=newelm;
	mLen=mLen+1;
	return true;
	}


////////////////////////////////////////////////
bool MPathNameList::Push(MPathNameList &paths)	// Push More Elements at end
	{
	int i;
	for(i=0;i<paths.GetElementCount();++i)
		{
		if(Push(paths.Get(i))==false) {  return false;  }
		}

	return true;
	}


////////////////////////////////////////////////
bool MPathNameList::Pop(void)						// Pop Element off list
	{
	if(mLen<=0)
		{
		return false;
		}

	mLen = mLen - 1;
	delete[] mList[mLen];
	mList[mLen]=NULL;
	return true;
	}


////////////////////////////////////////////////
bool MPathNameList::Pop(int cnt)				// Pop Till Count elements exist
	{

	while(mLen>cnt)
		{
		if(Pop()==false) { return false; }
		}

	return true;
	}

////////////////////////////////////////////////
const char *MPathNameList::GetTop(void)			// Get Last Element
	{
	if(mLen<=0)
		{
		return NULL;
		}

	return mList[mLen-1];
	}


///////////////////////////////////////////////
char MPathNameList::GetDriveLetter(void)			// Get Drive Letter
	{
	return mDriveLetter;
	}


////////////////////////////////////////////////
bool MPathNameList::IsSamePath(MPathNameList &lst)	// =true if paths are same
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
bool MPathNameList::IsSamePath(const char *path)		// =true if paths are same
	{
	if(path==NULL)
		{
		return false;
		}

	MPathNameList newpath;
	if(newpath.Create(path)==false)
		{
		return false;
		}

	return IsSamePath(newpath);
	}


