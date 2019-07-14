#pragma once

#include <atomic>
#include "Debug.h"

namespace z {

template<typename _class> static void InitializeSingleton(_class*& singleton_ptr, _class* instance) noexcept {
	void* ptr = (*(std::atomic<void*>*)(&singleton_ptr)).exchange(instance, std::memory_order_acq_rel);
	CHECK(ptr == nullptr);
}

template<typename _class> static void FinalizeSingleton(_class*& singleton_ptr, _class* instance) noexcept {
	void* ptr = (*(std::atomic<void*>*)(&singleton_ptr)).exchange(nullptr, std::memory_order_acq_rel);
}

}