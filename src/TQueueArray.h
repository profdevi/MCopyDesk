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


//v1.2 copyright Comine.com 20170625U1013
#ifndef TQueueArray_h
#define TQueueArray_h

////////////////////////////////////////////
#include "MStdLib.h"
#include "TVector.h"
#include "TAutoPtr.h"

/*
	Class implements a  queue of references pointers implemented in a fixed
	size array.
*/
////////////////////////////////////////////
template <class ItemClass>
class TQueueArray
	{
	TVector<TAutoPtr<ItemClass> > mArray;					
	int mFrontIndex;							// Current Index of the front
	int mLength;								// Length of Queue

	/////////////////////////////////////////
	void ClearObject(void)
		{
		mFrontIndex=0;
		mLength=0;
		}

	public:
	/////////////////////////////////////////
	TQueueArray(void)
		{  ClearObject();  }


	/////////////////////////////////////////
	explicit TQueueArray(int count)
		{
		ClearObject();
		if(Create(count)==false)
			{
			return;
			}
		}


	/////////////////////////////////////////
	~TQueueArray(void)
		{  Destroy();  }


	///////////////////////////////////////
	bool Create(int count)
		{
		Destroy();

		if(mArray.Create(count)==false)
			{
			Destroy();
			return false;
			}

		mFrontIndex=0;
		mLength=0;

		return true;
		}


	////////////////////////////////////////
	bool Destroy(void)
		{
		mArray.Destroy();
		ClearObject();
		return true;
		}


	///////////////////////////////////////
	int GetMaxCapacity(void) 
		{
		return mArray.GetCount();
		}


	///////////////////////////////////////
	int GetLength(void)
		{
		return mLength;
		}


	//////////////////////////////////////////////
	ItemClass *Get(int index)
		{
		MStdAssert(index>=0 && index<mLength );
		const int arraylength=mArray.GetCount();
		return mArray[(mFrontIndex+index)%arraylength].Get();
		}


	////////////////////////////////////////////////
	// Add item to end at mArray[mcCount]
	ItemClass *EnqueueBack(void)
		{
		const int arraylength=mArray.GetCount();
		if(mLength>=arraylength)
			{
			return NULL;
			}

		const int backindex=(mFrontIndex+mLength)%arraylength;

		if(mArray[backindex].Create()==false)
			{
			return NULL;
			}

		ItemClass *newitem=mArray[backindex].Get();
		MStdAssert(newitem!=NULL);

		mLength=mLength+1;
		return newitem;
		}


	////////////////////////////////////////////////
	// Add item to front of Queue
	ItemClass *EnqueueFront(void)
		{
		const int arraylength=mArray.GetCount();
		if(mLength>=arraylength)
			{
			return NULL;
			}

		mFrontIndex=(mFrontIndex+arraylength-1) % arraylength;
		mLength=mLength+1;

		if(mArray[mFrontIndex].Create()==false)
			{
			mFrontIndex=(mFrontIndex+1) % arraylength;
			return NULL;
			}

		ItemClass *newitem=mArray[mFrontIndex].Get();
		MStdAssert(newitem!=NULL);

		return newitem;
		}


	////////////////////////////////////////////////
	// Add item to end at mArray[mcCount]
	bool DequeueBack(void)
		{
		if(mLength<=0)
			{
			return false;
			}

		const int arraylength=mArray.GetCount();
		
		mLength=mLength-1;
		const int backindex=(mFrontIndex+mLength)%arraylength;

		mArray[backindex].Destroy();

		MStdAssert(mArray[backindex].Get()==NULL);
		
		return true;
		}


	////////////////////////////////////////////////
	bool DequeueFront(void)
		{
		if(mLength<=0)
			{
			return false;
			}

		const int arraylength=mArray.GetCount();
		mArray[mFrontIndex].Destroy();
		MStdAssert(mArray[mFrontIndex].Get()==NULL);

		mFrontIndex = (mFrontIndex + 1) % arraylength;
		mLength=mLength-1;
		return true;
		}

	};

#endif //TQueueArray_h

