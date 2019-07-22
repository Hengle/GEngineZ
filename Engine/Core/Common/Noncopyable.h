#pragma once

namespace z {

class NonCopyable {
public:
	NonCopyable() {}
	NonCopyable(NonCopyable&) = delete;
	NonCopyable& operator = (const NonCopyable&) = delete;
};

}