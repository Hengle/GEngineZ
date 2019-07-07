#pragma once

namespace z {


template<typename T>
class IList {
public:
	class ListNode {

	};
};


template<typename T>
class List: public IList<T> {

};


template<typename T>
class ThreadSafeList : public IList<T>{

};

template<typename T>
class LockFreeList : public IList<T> {

};


}
