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


//v1.1 copyright Comine.com 20210821S1811
#include <Windows.h>
#include <KnownFolders.h>
#include <ShlObj.h>
#include "MStdLib.h"
#include "MString.h"
#include "MStringArray.h"
#include "MWinFolders.h"


//******************************************************
//**  MWinFolders class
//******************************************************
void MWinFolders::ClearObject(void)
	{
	mpManager=NULL;
	mFolderCount=0;
	mpFolderIDs=NULL;
	}


////////////////////////////////////////////////
MWinFolders::MWinFolders(bool creat)
	{
	ClearObject();
	if(creat==true && Create()==false)
		{
		return;
		}
	}


////////////////////////////////////////////////
MWinFolders::~MWinFolders(void)
	{  Destroy();  }


////////////////////////////////////////////////
bool MWinFolders::Create(void)
	{
	Destroy();

	HRESULT hr=CoCreateInstance(CLSID_KnownFolderManager,NULL
			,CLSCTX_INPROC_SERVER,IID_PPV_ARGS(&mpManager) );
	if(FAILED(hr))
		{
		Destroy();
		return false;
		}

	MStdAssert(mpManager!=NULL);

	// Get Folder Count
	hr=mpManager->GetFolderIds(&mpFolderIDs,&mFolderCount);
	if(FAILED(hr))
		{
		Destroy();
		return false;
		}

	return true;
	}


////////////////////////////////////////////////
bool MWinFolders::Destroy(void)
	{
	if(mpFolderIDs!=NULL)
		{
		CoTaskMemFree(mpFolderIDs);
		mpFolderIDs=NULL;
		mFolderCount=0;
		}

	if(mpManager!=NULL)
		{
		mpManager->Release();
		mpManager=NULL;
		}

	ClearObject();
	return true;
	}


/////////////////////////////////////////////////
bool MWinFolders::GetFolderNames(MStringArray& foldernames)
	{
	if(foldernames.Create()==false)
		{
		return false;
		}

	for(UINT i=0;i<mFolderCount;++i)
		{
		IKnownFolder *pifolder=NULL;
		HRESULT hr=mpManager->GetFolder(mpFolderIDs[i],&pifolder);
		if(FAILED(hr))
			{ 
			continue;
			}

		KNOWNFOLDER_DEFINITION folderdef;
		MStdMemZero(&folderdef,sizeof(folderdef));

		hr=pifolder->GetFolderDefinition(&folderdef);
		if(FAILED(hr))
			{
			pifolder->Release();
			continue;
			}

		MString dirname;
		if(dirname.Create(folderdef.pszName)==false)
			{
			FreeKnownFolderDefinitionFields(&folderdef);
			pifolder->Release();
			continue;
			}
		
		if(foldernames.SetString(dirname.Get())==false)
			{
			// continue anyway
			}

		// Release Folder Definition fields
		FreeKnownFolderDefinitionFields(&folderdef);
	
		pifolder->Release();
		}

	return true;
	}

////////////////////////////////////////////////
bool MWinFolders::Print(void)
	{
	for(UINT i=0;i<mFolderCount;++i)
		{
		IKnownFolder *pifolder=NULL;
		HRESULT hr=mpManager->GetFolder(mpFolderIDs[i],&pifolder);
		if(FAILED(hr))
			{ 
			continue;
			}

		KNOWNFOLDER_DEFINITION folderdef;
		MStdMemZero(&folderdef,sizeof(folderdef));

		hr=pifolder->GetFolderDefinition(&folderdef);
		if(FAILED(hr))
			{
			pifolder->Release();
			continue;
			}

		//=We have Folder Definition
		if(folderdef.pszRelativePath!=NULL)
			{
			MStdPrintf("%d : %S : %S\n",i,folderdef.pszName,folderdef.pszRelativePath);
			}
		else
			{
			MStdPrintf("%d : %S : (NULL)\n", i, folderdef.pszName);
			}

		// Release Folder Definition fields
		FreeKnownFolderDefinitionFields(&folderdef);
	
		pifolder->Release();
		}

	return true;
	}


//////////////////////////////////////////////////////////////
bool MWinFolders::GetFolderPath(const char* foldername, MString& dirpath)
	{
	MStringWide widefoldername;
	if(widefoldername.Create(foldername)==false)
		{
		return false;
		}
	
	IKnownFolder *pfolder=NULL;
	HRESULT hr;
	hr=mpManager->GetFolderByName(widefoldername.Get(),&pfolder);
	if(FAILED(hr))
		{
		return false;
		}

	// ref: https://cpp.hotexamples.com/examples/-/IKnownFolder/GetPath/cpp-iknownfolder-getpath-method-examples.html
	LPWSTR ppath=NULL;
	hr=pfolder->GetPath(0,&ppath);
	if(FAILED(hr))
		{
		pfolder->Release();
		return false;
		}

	if(dirpath.Create(ppath)==false)
		{
		CoTaskMemFree(ppath);
		pfolder->Release();
		return false;
		}

	CoTaskMemFree(ppath);
	pfolder->Release();
	return true;
	}


