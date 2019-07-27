#pragma once

#include <cstdint>
#include <atomic>
#include "Define.h"

namespace z {

template<typename CounterT>
class RefCounterImpl {
public:
	RefCounterImpl(): mCnt(0) {}
	virtual ~RefCounterImpl() { CHECK(!mCnt); }
	
	int32_t AddRef() const {
		return ++mCnt;
	}

	int32_t Release() const {
		int old = --mCnt;
		if (mCnt == 0) {
			delete this;
		}
		return old;
	}

	int32_t RefCount() const{
		return mCnt;
	}

private:
	mutable CounterT mCnt;
};

typedef RefCounterImpl<int32_t> RefCounter;
typedef RefCounterImpl<std::atomic<int32_t>> ThreadSafeRefCounter;


template<typename RefType>
class RefCountPtr {
public:
	RefCountPtr(RefType *ptr = nullptr) : mPtr(ptr) {
		AddRef();
	}

	~RefCountPtr() {
		Reset();
	}

	RefCountPtr(const RefCountPtr& other) : RefCountPtr(other.mPtr) {
	}


	RefCountPtr(RefCountPtr&& other) {
		mPtr = other.mPtr;
		other.mPtr = nullptr;
	}

	RefCountPtr& operator=(RefType* ptr) {
		RefType* oldPtr = mPtr;
		mPtr = ptr;
		if (mPtr) mPtr->AddRef();
		if (oldPtr) oldPtr->Release();
		return *this;
	}

	RefCountPtr& operator=(const RefCountPtr& other) {
		return *this = other.mPtr;
	}

	RefCountPtr& operator=(RefCountPtr&& other) {
		if (this != &other) {
			RefType* oldPtr = mPtr;
			mPtr = other.mPtr;
			if (oldPtr) oldPtr->Release();
		}
		return *this;

	}
	RefType* operator->() const {
		return mPtr;
	}

	operator RefType* () const {
		return mPtr;
	}

	RefType** GetComRef () {
		// For COM
		*this = nullptr;
		return &mPtr;
	}

	RefType* GetRef() {
		return mPtr;
	}

	bool IsVaild() {
		return mPtr != nullptr;
	}

	void Reset() {
		if (mPtr) {
			mPtr->Release();
		}
		mPtr = nullptr;
	}

private:

	void AddRef() const {
		if (mPtr) {
			mPtr->AddRef();
		}
	}

	RefType* mPtr{nullptr};

};

}
