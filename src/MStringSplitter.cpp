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


//v2.1 copyright Comine.com 20150816U1925
#include "MStdLib.h"
#include "MBuffer.h"
#include "TVector.h"
#include "MStringSplitter.h"


//******************************************************
//**  MStringSplitter class
//******************************************************
void MStringSplitter::ClearObject(void)
	{
	mCount=0;
	}


////////////////////////////////////////////////
MStringSplitter::MStringSplitter(void)
	{  ClearObject();  }


///////////////////////////////////////////////
MStringSplitter::MStringSplitter(const char *str,const char *splitchars)
	{
	ClearObject();
	if(Create(str,splitchars)==false)
		{
		return;
		}
	}


////////////////////////////////////////////////
MStringSplitter::~MStringSplitter(void)
	{  Destroy();  }


////////////////////////////////////////////////
bool MStringSplitter::Create(const char *str,const char *splitchars)
	{
	Destroy();

	// Check arguments
	if(str==NULL || splitchars==NULL || *splitchars==0)
		{
		Destroy();
		return false;
		}

	// Allocate and copy space for string
	const int length=MStdStrLen(str);

	if(mString.Create(length+1)==false)
		{
		Destroy();
		return false;
		}

	// Copy the string into fresh space
	MStdStrCpy(mString.GetBuffer(),str);

	//*Set all split characters to zero
	const int splitlength=MStdStrLen(splitchars);
	int i,k;
	for(i=0;i<length;++i)
		{
		for(k=0;k<splitlength;++k)
			{
			if(mString[i]!=splitchars[k]) { continue; }
			mString[i]=0;  break;
			}
		}

	//*Now Count of split strings
	int count=0;  
	int state=0;
	for(i=0;i<=length;++i)
		{
		if(state==0 && mString[i]!=0)
			{  count=count+1;  state=1; }
		else if(state==1 && mString[i]==0)
			{  state=0;  }
		}

	// Assign 
	mCount=count;

	if(mArray.Create(mCount+1)==false)
		{
		Destroy();
		return false;
		}

	if(count==0)
		{
		// Return success ( No SPlits)
		mArray[0]=mString.GetBuffer();
		return true;
		}

	// Now Set the array
	state=0; count=0;
	for(i=0;i<length;++i)
		{
		if(state==0 && mString[i]!=0)
			{
			state=1;
			mArray[count]=mString+i;
			count=count+1;
			}
		else if(state==1 && mString[i]==0)
			{  state=0;  }
		}

	return true;
	}


////////////////////////////////////////////////
bool MStringSplitter::Destroy(void)
	{
	mString.Destroy();
	mArray.Destroy();
	ClearObject();
	return true;
	}


////////////////////////////////////////////////
int MStringSplitter::GetCount(void)							// Get Count of split strings
	{
	return mCount;
	}


////////////////////////////////////////////////
const char *MStringSplitter::Get(int index)					// Get Split String
	{
	return mArray[index];
	}


