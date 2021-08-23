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
#ifndef MFilePathBuilder_h
#define MFilePathBuilder_h

//////////////////////////////////////////////////
#include "MStdLib.h"
#include "MBuffer.h"
#include "MString.h"
#include "TQueueArray.h"

/*
	Class can be used to build paths
	Output Path will have generally /a/b/c format accept in windows
	where no leading / is placed for drives.

	Cannot Push . or .. as path elements

*/

//******************************************************
//**  MFilePathBuilder class
//******************************************************
class MFilePathBuilder
	{
	////////////////////////////////////////////////
	TQueueArray<MString> mList;
	
	////////////////////////////////////////////////
	void ClearObject(void);

	////////////////////////////////////////////////
	public:
	MFilePathBuilder(bool create=false);
	MFilePathBuilder(const char *dirpath);
	~MFilePathBuilder(void);
	bool Create(void);
	bool Create(const char *absdirpath);
	bool Destroy(void);
	const char *Get(int index);
	int GetElementCount(void);
	bool GetFullPath(MBuffer &buffer);	
	bool Push(const char *element);			// Push Element into list
	bool Push(MFilePathBuilder &paths);		// Push More Elements at end
	bool Pop(void);							// Pop Element off list
	bool Pop(int count);					// Pop Till Count elements exist
	const char *GetTop(void);				// Get Last Element
	bool IsSamePath(MFilePathBuilder &lst);	// =true if paths are same
	bool IsSamePath(const char *path);		// =true if paths are same
	};

#endif // MFilePathBuilder_h

