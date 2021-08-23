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


//v1.20 copyright Comine.com 20150813R1238
#ifndef MDirOps_h
#define MDirOps_h

////////////////////////////////////////////////////
#include "MBuffer.h"

//******************************************************
//**  MDirOps class
//******************************************************
class MDirOps
	{
	////////////////////////////////////////////////
	
	////////////////////////////////////////////////
	void ClearObject(void);
	
	////////////////////////////////////////////////
	public:
	MDirOps(bool create=false);
	~MDirOps(void);
	bool Create(void);
	bool Destroy(void);
	bool MakeDir(const char *dirname,bool erroronfail=false);	// Make a directory

	// Make all subdirectories.  For instance c:\work\tmp\x\y will create
	// subdirectories c:\work, c:\work\tmp, c:\work\tmp\x, c:\work\tmp\x\y
	bool MakeDirNested(const char *dirname);

	bool RemoveDir(const char *dirname,bool erroronfail=false);
	bool Exists(const char *dirname);
	bool IsSystem(const char *dirname);
	bool IsReadOnly(const char *dirname);
	bool IsHidden(const char *dirname);
	bool IsEncrypted(const char *dirname);
	bool IsCompressed(const char *dirname);						// Is Directory Compressed
	bool Move(const char *srcdir,const char *dstdir				// Move a directory
			,bool erroronfail=false);
	bool MoveOnReboot(const char *srcdir,const char *dstdir);	// Move dir after reboot
	bool GetCurrentDir(MBuffer &dir);							// Return Current Directory
	bool GetCurrentDir(char drive,MBuffer &dir);				// Return Current Directory of drive
	bool GetTempDir(MBuffer &buf);								// Get Temp Directory with \ end
	bool SetDir(const char *dir);								// Change Directory
	bool IsReadable(const char *dir);							// Is Directory Readable
	};

#endif // MDirOps_h

