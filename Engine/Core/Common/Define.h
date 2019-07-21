#pragma once

#include "Logger.h"

// define some useful macro


#define SAFE_DELETE(x) do {	\
	delete x;				\
	x = nullptr;			\
} while (0);

#define CHECK(expr, ...) \
	if (!(expr)) z::Log<z::LFATAL>(__FILE__, __LINE__, ##__VA_ARGS__);