#pragma once

#include <sstream>
#include <iostream>

namespace z {

enum LogLevel {
	LDEBUG = 0,
	LINFO,
	LWARN,
	LERROR,
	LFATAL,
};

std::string const LogStr[] = {
	"DEBUG",
	"INFO",
	"WARN",
	"ERROR",
	"FATAL",
};

template<LogLevel LEVEL>
constexpr auto GetLogString() {
	if constexpr (LEVEL == LDEBUG) {
		return "DEBUG";
	}
	if constexpr (LEVEL == LINFO) {
		return "INFO";
	}
	if constexpr (LEVEL == LWARN) {
		return "WARN";
	}
	if constexpr (LEVEL == LERROR) {
		return "ERROR";
	}
	return "FATAL";
}

template<LogLevel LEVEL>
class Log {
public:

	template<typename... Args>
	Log(Args... args){
		stream_ << GetLogString<LEVEL>() << " ";
		if constexpr(sizeof...(args) > 0)
			Print(args...);
	}

	template<typename T, typename... Args>
	void Print(T first, Args... args) {
		stream_ << first << " ";
		if constexpr (sizeof...(args) > 0) {
			Print(args...);
		}
	}

	~Log() {
		stream_ << std::endl;
		std::wcout << stream_.str();
		if constexpr (LEVEL == LFATAL) {
			exit(-1);
		}
	}

	template<typename T>
	Log& const operator << (T&& t) {
		stream_ << t;
		return *this;
	}

private:
	std::wstringstream stream_;
};


}