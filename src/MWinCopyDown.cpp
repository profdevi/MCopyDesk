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


//v1.0 copyright Comine.com 20210822U2059
#include "MStdLib.h"
#include "MString.h"
#include "MWinFolders.h"
#include "MWUDirReader.h"
#include "MWinCopyDown.h"


//******************************************************
//**  MWinCopyDown class
//******************************************************
void MWinCopyDown::ClearObject(void)
	{
	}


////////////////////////////////////////////////
MWinCopyDown::MWinCopyDown(bool creat)
	{
	ClearObject();
	if(creat==true && Create()==false)
		{
		return;
		}
	}


////////////////////////////////////////////////
MWinCopyDown::~MWinCopyDown(void)
	{  Destroy();  }


////////////////////////////////////////////////
bool MWinCopyDown::Create(void)
	{
	Destroy();

	MWinFolders winfolders;
	if(winfolders.Create()==false)
		{
		Destroy();
		return false;
		}

	if(winfolders.GetFolderPath("Downloads",mDownloadFolder)==false)
		{
		Destroy();
		return false;
		}

	if(MStdDirExists(mDownloadFolder.Get())==false)
		{
		Destroy();
		return false;
		}

	return true;
	}


////////////////////////////////////////////////
bool MWinCopyDown::Destroy(void)
	{
	mDownloadFolder.Destroy();
	ClearObject();
	return true;
	}


////////////////////////////////////////////////
bool MWinCopyDown::PrintDownloads(void)
	{
	MWUDirReader dirreader;
	if(dirreader.Create(mDownloadFolder.Get())==false)
		{
		return false;
		}

	for(int i=0;dirreader.Read()==true;++i)
		{
		time_t createtime;
		dirreader.GetCreateTime(createtime);
		char timebuf[100];
		MStdCTime(timebuf,sizeof(timebuf)-2,&createtime);
		MStdStrClean(timebuf);

		MStdPrintf("%2d : %-50s %10d  %s\n",i
			,dirreader.GetFileName(),dirreader.GetFileSizeLow(),timebuf);
		}

	return true;
	}


/////////////////////////////////////////////////////////
bool MWinCopyDown::CopyFromDown(int fileindex,const char *targetfilename)
	{
	MWUDirReader dirreader;
	if(dirreader.Create(mDownloadFolder.Get())==false)
		{
		return false;
		}

	for(int i=0;dirreader.Read()==true;++i)
		{
		if(i!=fileindex) { continue; }

		//=Found File
		const char *filepath=dirreader.GetFullFilePath();
		
		if(targetfilename==0)
			{
			targetfilename=dirreader.GetFileName();
			}

		if(MStdFileCopy(filepath,targetfilename,false,true)==true)
			{
			MStdPrintf("Copied %s from Downloads\n",dirreader.GetFileName());
			}
		else
			{
			MStdPrintf("--Failed copy of %s\n",dirreader.GetFileName());
			}
		return true;
		}
	
	return true;
	}


