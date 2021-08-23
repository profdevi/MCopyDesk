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


//v1.50 copyright Comine.com 20190829R1212
#ifndef MFileOps_h
#define MFileOps_h

////////////////////////////////////////////////
#include "MStdLib.h"
#include "MString.h"
#include "MBuffer.h"

//******************************************************
//**  MFileOps class
//******************************************************
class MFileOps
	{
	////////////////////////////////////////////////
	void ClearObject(void);

	////////////////////////////////////////////////
	public:
	MFileOps(void);
	explicit MFileOps(bool autocreate);
	~MFileOps(void);
	bool Create(void);
	bool Destroy(void);
	bool Move(const char *srcfile,const char *dstfile			// Move a file
			,bool erroronfail=false);			
	bool MoveOnReboot(const char *srcfile,const char *dstfile);	// Move File after reboot
	bool Delete(const char *filename							// Delete a file
			,bool erroronfaile=false);
	bool DeleteOnReboot(const char *filename);					// Delete file on reboot
	bool OverWriteDelete(const char *filename);					// Overwrite and delete file
	bool Copy(const char *srcfile,const char *dstfile			// Copy a file
			,bool stopifexists=false,bool erroronfail=false);					
	bool Exists(const char *filename);							// Check if file exists
	bool IsBinary(const char *filename);						// Check if file is binary
	bool IsOpenable(const char *filename);						// Check if file is openable
	bool AreSame(const char *file1,const char *file2);			// Compare two files
	bool ConvertTextUnixToDos(const char *filename);			// Unix To Dos Text
	bool ConvertTextDosToUnix(const char *filename);			// Dos To Unix Text
	bool CleanToDosText(const char *filename);					// Cleans to Dos Text
	bool IsUnixTextFile(const char *filename);					// Check if unix text file
	bool IsDosTextFile(const char *filename);					// Check of dos text file
	bool Append(const char *filename,const char *extrafile);	// Append contents of file with extra
	bool MakeNewFile(const char *filename);						// Create fresh blank file
	bool IsDirectory(const char *filename);						// = true if filename is dir
	bool IsReadOnly(const char *filename);						// =true if filename
	bool SetReadOnly(const char *filename,bool val);			// Set File name
	bool IsSystemFile(const char *filename);					// =true if filename
	bool SetSystemFile(const char *filename,bool val);			// Set File as system file
	bool IsHiddenFile(const char *filename);					// =true if filename
	bool SetHiddenFile(const char *filename,bool val);			// Set File as hidden file
	bool WriteTextFile(const char *filename,const char *data);	// Create fresh file
	bool WriteBinaryFile(const char *filename,const char *data,int buffer);
	bool AppendTextFile(const char *filename,const char *extra);// Append text to file
	bool ReadTextFile(const char *filename,char *buffer
			,int &size);										// Read from file into buffer
	bool ReadTextFile(const char *filename,MBuffer &buffer);	// Read from file into buffer
	bool ReadBinaryFile(const char *filename,MBuffer &buffer);	// Read from file into buffer
	bool GetAbsolutePath(const char *filename
			,MString &absolutepath);							// Get Path for file
	bool GetTemperoryFileName(MString &filename);				// Get Full path Temp File
	bool WriteToExistingFile(const char *filename
			,const char *data,int length,int offset=0);			// Write to existing binary file
	bool GetTempFileNameInDir(const char *directory,MString &filename);
	bool GetCanonicalName(const char *path,MString &canonname);	// convert all paths to / based
	bool PrintFile(const char *filename);						// Print contents of file
	int GetSize(const char *filename);							// Get File Size
	bool SetSize(const char *filename,int size);				// Resize file
	int GetLineCount(const char *filename);						// Get Line Count of file
	bool GetAppPath(MString &apppathname);						// Get Application path name
	bool GetAppConfigFile(MString &configfile);					// Get Application Configuration File
	};

#endif // MFileOps_h

