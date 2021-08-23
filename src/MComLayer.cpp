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


//v1.5 copyright Comine.com 20160719T1258
#define _WIN32_DCOM
#include <windows.h>
#include "MStdLib.h"
#include "MComLayer.h"


////////////////////////////////////////////////////
#pragma comment(lib,"ole32.lib")
#pragma comment(lib,"oleaut32.lib")

//******************************************************
//**  MComLayer class
//******************************************************
void MComLayer::ClearObject(void)
	{
	mInitialized=false;
	}


////////////////////////////////////////////////
MComLayer::MComLayer(void)
	{  ClearObject();  }


////////////////////////////////////////////////
MComLayer::MComLayer(DWORD init)
	{
	ClearObject();
	if(Create(init)==false)
		{
		return;
		}
	}


////////////////////////////////////////////////
MComLayer::~MComLayer(void)
	{  Destroy();  }


////////////////////////////////////////////////
bool MComLayer::Create(DWORD init)
	{
	Destroy();

	HRESULT hret=CoInitializeEx(NULL,init);
	if(FAILED(hret)==TRUE)
		{
		Destroy();
		return false;
		}

	mInitialized=true;
	return true;
	}


////////////////////////////////////////////////
bool MComLayer::Destroy(void)
	{
	if(mInitialized==true) {  CoUninitialize(); }

	ClearObject();
	return true;
	}


