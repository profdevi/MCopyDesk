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


//v0.0 copyright Comine.com 20190829R1147
#include "MStdLib.h"
#include "MCommandArg.h"
#include "MComLayer.h"
#include "MWinCopyDown.h"

//******************************************************
//* Module Elements
//******************************************************
static const char *GApplicationName="MCopyDown";	// Used in Help
static const char *GApplicationVersion="1.0";	// Used in Help

////////////////////////////////////////////////////
static void GDisplayHelp(void);


////////////////////////////////////////////////////
int main(int argn,const char *argv[])
	{
	MCommandArg args(argn,argv);

	if(args.CheckRemoveHelp()==true)
		{
		GDisplayHelp();
		return 0;
		}

	MComLayer comlayer;
	comlayer.Create();

	MWinCopyDown copymgr(true);

	const int argcount=args.GetArgCount();
	if(argcount==1)
		{
		copymgr.PrintDownloads();
		return 0;
		}
	else if(argcount==2)
		{
		const int fileindex=MStdAToI(args.GetArg(1));
		copymgr.CopyFromDown(fileindex,0);
		return 0;
		}
	else 
		{
		const int fileindex=MStdAToI(args.GetArg(1) );
		copymgr.CopyFromDown(fileindex,args.GetArg(2));
		return 0;
		}

	return 0;
	}


////////////////////////////////////////////////////
static void GDisplayHelp(void)
	{
	MStdPrintf(	"\n"
				"   usage:  %s [-?] [<fileindex>] [<fileindex> <filename>]\n"
				"           v%s copyright Comine.com\n"
				"\n"
				"   Program will copy files from the downloads folder to current directory\n"
				"\n"
				"   options:\n"
				"        <fileindex>  :  File Index for copy\n"
				"        <filename>   :  Renamed file\n"
				"\n"
				"   Example #1: Show file in Downloads folder\n"
				"            c:>  %s\n"
				"\n"				
				"   Example #2: Copy 4th file to current directory\n"
				"            c:>  %s 3\n"
				"\n"
				"   Example #3: Copy First file and rename it to one.jpg\n"
				"            c:>  %s 0 one.jpg\n"
				"\n"
				,GApplicationName,GApplicationVersion,GApplicationName
				,GApplicationName,GApplicationName);
	}


